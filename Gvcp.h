#ifndef GVCP_H
#define GVCP_H

#include <memory>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

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

class GigEVision;

class GvcpManager
{

  public:
  GvcpManager(GigEVision& gev);

  ~GvcpManager();

  void Connect(const std::string& sAddr, const std::string& sPort);

  uint32_t Read(const std::string& sKey);
  uint32_t Read(uint32_t addr);

  std::vector<uint8_t> ReadBlock(const std::string& sKey, uint32_t nSize);
  std::vector<uint8_t> ReadBlock(uint32_t addr, uint32_t nSize);

  bool Write(const std::string& sKey, uint32_t nVal);
  bool Write(uint32_t addr, uint32_t nVal);

  bool RequestResend(uint32_t nImage, uint32_t nStartPack, uint32_t nEndPack);

  boost::asio::ip::address_v4 FindCam();

  void StartHeartbeat();

  void StopHeartbeat();

  private:
  void Heartbeat();
  GigEVision& m_gev;
  boost::asio::ip::udp::socket m_sock;
  boost::asio::ip::udp::resolver::iterator m_it;
  std::auto_ptr<boost::asio::deadline_timer> m_heartbeatTimer;
  boost::thread m_ioThread;
  int m_nMsgNr;
  bool m_bHeartbeatRun;
};


#endif // GVCP_H
