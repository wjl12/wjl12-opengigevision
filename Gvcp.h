#ifndef GVCP_H
#define GVCP_H

#include <memory>
#include <asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

enum EnAddrs
{
  ACCESS_CAM = 0x00000a00,
  DATASTREAM_PORT = 0x00000d00,
  DATASTREAM_ADDRESS = 0x00000d18,
  DATASTREAM_PACKET_SIZE = 0x00000d04,
  START_GRAB = 0x00010204,
  STOP_GRAB = 0x00010204,
  GRABBING_STOPPED = 0x0001020c,
  PIXEL_FORMAT = 0x0001010c,
  GENICAM_ZIPFILEINFO_ADDRESS = 0x00000200
};

enum EnVals
{
  ACCESS_CAM_VALUE = 0x03,
  STOP_GRAB_VALUE = 0x01000000,
  PIXEL_FORMAT_8BIT_VALUE = 0x01000801,
  PIXEL_FORMAT_12BIT_VALUE = 0x01000c01
};



class GvcpManager
{

  public:

  GvcpManager(asio::io_service& service, const std::string& sAddr, const std::string& sPort):
      m_service(service),
      m_sock(m_service, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0)),
      m_heartbeatTimer(new asio::deadline_timer(m_service, boost::posix_time::millisec(700))),
      m_nMsgNr(1),
      m_bHeartbeatRun(false)
  {
    asio::ip::udp::resolver resolver(m_service);

    asio::ip::udp::resolver_query query(asio::ip::udp::v4(), sAddr, sPort);
    m_it = resolver.resolve(query);
  }

  ~GvcpManager()
  {
    m_bHeartbeatRun = false;
    // TODO: add wait for finish
  }

  uint32_t Read(EnAddrs addr)
  {
    boost::array<char, 12> buff = {{ 0x42, 0x01, 0x00, 0x80, 0x00, 0x04 }};
    ((uint16_t*)&buff)[3] = htons(m_nMsgNr++);
    ((uint32_t*)&buff)[2] = htonl(addr);
    m_sock.send_to(asio::buffer(buff), *m_it);

    boost::array<char, 256> recv_buf;
    asio::ip::udp::endpoint sender_endpoint;
    /*size_t len =*/ m_sock.receive_from(asio::buffer(recv_buf), sender_endpoint);

    return htonl(((uint32_t*)&recv_buf)[2]);
  }

  std::vector<char> ReadBlock(int addr, uint32_t nSize)
  {
    static const int cnHdLen = 12;
    boost::array<char, 16> buff = {{ 0x42, 0x01, 0x00, 0x84, 0x00, 0x08 }};
    std::vector<char> retVec;
    boost::array<char, 528> recv_buf;
    size_t len = 0;

    do
    {
      ((uint16_t*)&buff)[3] = htons(m_nMsgNr++);
      ((uint32_t*)&buff)[2] = htonl(addr + len);
      ((uint32_t*)&buff)[3] = htonl((nSize-len < recv_buf.size()-cnHdLen ? nSize-len: recv_buf.size()-cnHdLen));
      m_sock.send_to(asio::buffer(buff), *m_it);

      asio::ip::udp::endpoint sender_endpoint;
      size_t nRead = m_sock.receive_from(asio::buffer(recv_buf), sender_endpoint) - cnHdLen;
      if(nSize - len < recv_buf.size() - cnHdLen)
        nRead = nSize - len;

      len += nRead;
      std::copy(recv_buf.begin()+cnHdLen, recv_buf.begin() + cnHdLen + nRead, std::back_inserter(retVec));
    }
    while(len < nSize);

    return retVec;
  }

  bool Write(EnAddrs addr, uint32_t nVal)
  {
    boost::array<char, 16> buff = {{ 0x42, 0x01, 0x00, 0x82, 0x00, 0x08 }};
    ((uint16_t*)&buff)[3] = htons(m_nMsgNr++);
    ((uint32_t*)&buff)[2] = htonl(addr);
    ((uint32_t*)&buff)[3] = htonl(nVal);
    m_sock.send_to(asio::buffer(buff), *m_it);

    boost::array<char, 256> recv_buf;
    asio::ip::udp::endpoint sender_endpoint;
    if(m_sock.receive_from(asio::buffer(recv_buf), sender_endpoint) != 12)
      return false;
    else
      return recv_buf[11] == 0x01;
  }


  asio::ip::address_v4 FindCam()
  {
    boost::array<char, 8> buff = {{ 0x42, 0x01, 0x00, 0x02, 0x00, 0x00 }};
    ((uint16_t*)&buff)[3] = htons(m_nMsgNr++);
    m_sock.send_to(asio::buffer(buff), *m_it);

    boost::array<char, 256> recv_buf;
    asio::ip::udp::endpoint sender_endpoint;
    m_sock.receive_from(asio::buffer(recv_buf), sender_endpoint);
    asio::ip::address_v4 addr(recv_buf[47] + ((uint32_t(recv_buf[46]))<<8) +
                              ((uint32_t(recv_buf[45]))<<16) + ((uint32_t(recv_buf[44]))<<24));
    return addr;
  }

  void StartHeartbeat()
  {
    m_bHeartbeatRun = true;
    m_heartbeatTimer->async_wait(boost::bind(&GvcpManager::Heartbeat, this));
    m_ioThread = boost::thread(boost::bind(&asio::io_service::run, &m_service));
  }

  void StopHeartbeat()
  {
    m_bHeartbeatRun = false;
    m_ioThread.join();
  }

  private:

  void Heartbeat()
  {
    std::cout << "Bing! " << (Read(ACCESS_CAM) == 0x3) << std::endl;

    if(m_bHeartbeatRun == true)
    {
      m_heartbeatTimer->expires_at(m_heartbeatTimer->expires_at() + boost::posix_time::millisec(700));
      m_heartbeatTimer->async_wait(boost::bind(&GvcpManager::Heartbeat, this));
    }
  }

  asio::io_service& m_service;
  asio::ip::udp::socket m_sock;
  asio::ip::udp::resolver::iterator m_it;
  std::auto_ptr<asio::deadline_timer> m_heartbeatTimer;
  boost::thread m_ioThread;
  int m_nMsgNr;
  bool m_bHeartbeatRun;
};


#endif // GVCP_H
