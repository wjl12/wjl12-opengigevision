#include <boost/date_time/posix_time/posix_time.hpp>

#include "GigEVision.h"
#include "GenICam.h"
#include "Gvcp.h"

GvcpManager::GvcpManager(GigEVision& gev):
    m_gev(gev),
    m_sock(m_gev.Service(), boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0)),
    m_heartbeatTimer(new boost::asio::deadline_timer(gev.Service(), boost::posix_time::millisec(700))),
    m_nMsgNr(1),
    m_bHeartbeatRun(false)
{

}

void GvcpManager::Connect(const std::string& sAddr, const std::string& sPort)
{
  m_nMsgNr = 1;
  boost::asio::ip::udp::resolver resolver(m_gev.Service());

  boost::asio::ip::udp::resolver_query query(boost::asio::ip::udp::v4(), sAddr, sPort);
  m_it = resolver.resolve(query);
}

GvcpManager::~GvcpManager()
{
  m_bHeartbeatRun = false;
  // TODO: add wait for finish
}

uint32_t GvcpManager::Read(const std::string& sKey)
{
  return Read(m_gev.GenICam().GetAddress(sKey));
}

uint32_t GvcpManager::Read(uint32_t addr)
{
  boost::array<uint8_t, 12> buff = {{ 0x42, 0x01, 0x00, 0x80, 0x00, 0x04 }};
  ((uint16_t*)&buff)[3] = htons(m_nMsgNr++);
  ((uint32_t*)&buff)[2] = htonl(addr);
  m_sock.send_to(boost::asio::buffer(buff), *m_it);

  boost::array<uint8_t, 256> recv_buf;
  boost::asio::ip::udp::endpoint sender_endpoint;
  if(m_sock.receive_from(boost::asio::buffer(recv_buf), sender_endpoint) != 12)
    throw std::runtime_error("Error reading data from camera");

  return htonl(((uint32_t*)&recv_buf)[2]);
}

std::vector<uint8_t> GvcpManager::ReadBlock(const std::string& sKey, uint32_t nSize)
{
  // todo: expensive duplicate copy of memory, perhaps change interface to pass
  // the vector as parameter by reference
  return ReadBlock(m_gev.GenICam().GetAddress(sKey), nSize);
}

std::vector<uint8_t> GvcpManager::ReadBlock(uint32_t addr, uint32_t nSize)
{
  static const int cnHdLen = 12;
  boost::array<uint8_t, 16> buff = {{ 0x42, 0x01, 0x00, 0x84, 0x00, 0x08 }};
  std::vector<uint8_t> retVec;
  boost::array<uint8_t, 528> recv_buf;
  size_t len = 0;

  do
  {
    ((uint16_t*)&buff)[3] = htons(m_nMsgNr++);
    ((uint32_t*)&buff)[2] = htonl(addr + len);
    ((uint32_t*)&buff)[3] = htonl((nSize-len < recv_buf.size()-cnHdLen ? nSize-len: recv_buf.size()-cnHdLen));
    m_sock.send_to(boost::asio::buffer(buff), *m_it);

    boost::asio::ip::udp::endpoint sender_endpoint;
    size_t nRead = m_sock.receive_from(boost::asio::buffer(recv_buf), sender_endpoint) - cnHdLen;
    len += nRead;
    std::copy(recv_buf.begin() + cnHdLen, recv_buf.begin() + cnHdLen + nRead, std::back_inserter(retVec));
  }
  while(len < nSize);

  return retVec;
}

bool GvcpManager::Write(const std::string& sKey, uint32_t nVal)
{
  return Write(m_gev.GenICam().GetAddress(sKey), nVal);
}

bool GvcpManager::Write(uint32_t addr, uint32_t nVal)
{
  boost::array<uint8_t, 16> buff = {{ 0x42, 0x01, 0x00, 0x82, 0x00, 0x08 }};
  ((uint16_t*)&buff)[3] = htons(m_nMsgNr++);
  ((uint32_t*)&buff)[2] = htonl(addr);
  ((uint32_t*)&buff)[3] = htonl(nVal);
  m_sock.send_to(boost::asio::buffer(buff), *m_it);

  boost::array<uint8_t, 12> recv_buf;
  boost::asio::ip::udp::endpoint sender_endpoint;
  if(m_sock.receive_from(boost::asio::buffer(recv_buf), sender_endpoint) != 12)
    return false;
  else
    return recv_buf[11] == 0x01;
}

bool GvcpManager::RequestResend(uint32_t nImage, uint32_t nStartPack, uint32_t nEndPack)
{
  boost::array<uint8_t, 20> buff = {{ 0x42, 0x00, 0x00, 0x40, 0x00, 0x0c }};
  ((uint16_t*)&buff)[3] = htons(m_nMsgNr++);
  ((uint32_t*)&buff)[2] = htonl(nImage);
  ((uint32_t*)&buff)[3] = htonl(nStartPack);
  ((uint32_t*)&buff)[4] = htonl(nEndPack);
  m_sock.send_to(boost::asio::buffer(buff), *m_it);

  boost::array<uint8_t, 12> recv_buf;
  boost::asio::ip::udp::endpoint sender_endpoint;
  if(m_sock.receive_from(boost::asio::buffer(recv_buf), sender_endpoint) != 12)
    return false;
  else
    return recv_buf[11] == 0x01;
}

boost::asio::ip::address_v4 GvcpManager::FindCam()
{
  boost::array<uint8_t, 8> buff = {{ 0x42, 0x01, 0x00, 0x02, 0x00, 0x00 }};
  ((uint16_t*)&buff)[3] = htons(m_nMsgNr++);
  m_sock.send_to(boost::asio::buffer(buff), *m_it);

  boost::array<uint8_t, 256> recv_buf;
  boost::asio::ip::udp::endpoint sender_endpoint;
  m_sock.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
  boost::asio::ip::address_v4 addr(recv_buf[47] + ((uint32_t(recv_buf[46]))<<8) +
                                   ((uint32_t(recv_buf[45]))<<16) + ((uint32_t(recv_buf[44]))<<24));
  return addr;
}

void GvcpManager::StartHeartbeat()
{
  m_bHeartbeatRun = true;
  m_heartbeatTimer->async_wait(boost::bind(&GvcpManager::Heartbeat, this));
  m_ioThread = boost::thread(boost::bind(&boost::asio::io_service::run, &m_gev.Service()));
}

void GvcpManager::StopHeartbeat()
{
  m_bHeartbeatRun = false;
  m_ioThread.join();
}

void GvcpManager::Heartbeat()
{
  if(m_bHeartbeatRun == true)
  {
    m_heartbeatTimer->expires_at(m_heartbeatTimer->expires_at() + boost::posix_time::millisec(700));
    m_heartbeatTimer->async_wait(boost::bind(&GvcpManager::Heartbeat, this));
  }
}
