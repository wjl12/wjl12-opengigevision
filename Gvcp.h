#ifndef GVCP_H
#define GVCP_H

#include <memory>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

enum EnAddrs
{
  DATASTREAM_PACKET_SIZE = 0x00000d04,
  GRABBING_STOPPED = 0x0001020c,
  PIXEL_FORMAT = 0x0001010c,
  GENICAM_ZIPFILEINFO_ADDRESS = 0x00000200
};

enum EnVals
{
  EXCLUSIVE_ACCESS = 0x03,
  STOP_GRAB_VALUE = 0x01000000,
  PIXEL_FORMAT_8BIT_VALUE = 0x01000801,
  PIXEL_FORMAT_12BIT_VALUE = 0x01000c01
};



class GvcpManager
{

  public:

  GvcpManager(boost::asio::io_service& service, const std::string& sAddr, const std::string& sPort):
      m_service(service),
      m_sock(m_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0)),
      m_heartbeatTimer(new boost::asio::deadline_timer(m_service, boost::posix_time::millisec(700))),
      m_nMsgNr(1),
      m_bHeartbeatRun(false)
  {
    boost::asio::ip::udp::resolver resolver(m_service);

    boost::asio::ip::udp::resolver_query query(boost::asio::ip::udp::v4(), sAddr, sPort);
    m_it = resolver.resolve(query);
  }

  ~GvcpManager()
  {
    m_bHeartbeatRun = false;
    // TODO: add wait for finish
  }

  uint32_t Read(uint32_t addr)
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

  std::vector<uint8_t> ReadBlock(uint32_t addr, uint32_t nSize)
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

  bool Write(uint32_t addr, uint32_t nVal)
  {
    boost::array<uint8_t, 16> buff = {{ 0x42, 0x01, 0x00, 0x82, 0x00, 0x08 }};
    ((uint16_t*)&buff)[3] = htons(m_nMsgNr++);
    ((uint32_t*)&buff)[2] = htonl(addr);
    ((uint32_t*)&buff)[3] = htonl(nVal);
    m_sock.send_to(boost::asio::buffer(buff), *m_it);

    boost::array<uint8_t, 256> recv_buf;
    boost::asio::ip::udp::endpoint sender_endpoint;
    if(m_sock.receive_from(boost::asio::buffer(recv_buf), sender_endpoint) != 12)
      return false;
    else
      return recv_buf[11] == 0x01;
  }


  boost::asio::ip::address_v4 FindCam()
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

  void StartHeartbeat()
  {
    m_bHeartbeatRun = true;
    m_heartbeatTimer->async_wait(boost::bind(&GvcpManager::Heartbeat, this));
    m_ioThread = boost::thread(boost::bind(&boost::asio::io_service::run, &m_service));
  }

  void StopHeartbeat()
  {
    m_bHeartbeatRun = false;
    m_ioThread.join();
  }

  private:

  void Heartbeat()
  {
    if(m_bHeartbeatRun == true)
    {
      m_heartbeatTimer->expires_at(m_heartbeatTimer->expires_at() + boost::posix_time::millisec(700));
      m_heartbeatTimer->async_wait(boost::bind(&GvcpManager::Heartbeat, this));
    }
  }

  boost::asio::io_service& m_service;
  boost::asio::ip::udp::socket m_sock;
  boost::asio::ip::udp::resolver::iterator m_it;
  std::auto_ptr<boost::asio::deadline_timer> m_heartbeatTimer;
  boost::thread m_ioThread;
  int m_nMsgNr;
  bool m_bHeartbeatRun;
};


#endif // GVCP_H
