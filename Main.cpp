#include <iostream>
#include <exception>
#include <asio.hpp>
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <vigra/stdimage.hxx>
#include <vigra/impex.hxx>

#include "Gvsp.h"

int gnMsgNr = 1;
bool gbHeartbeatRun = false;

enum EnAddrs
{
  ACCESS_CAM = 0x00000a00,
  DATASTREAM_PORT = 0x00000d00,
  DATASTREAM_ADDRESS = 0x00000d18,
  DATASTREAM_PACKET_SIZE = 0x00000d04,
  START_GRAB = 0x00010204,
  STOP_GRAB = 0x00010204,
  GRABBING_STOPPED = 0x0001020c,
  PIXEL_FORMAT = 0x0001010c
};

enum EnVals
{
  ACCESS_CAM_VALUE = 0x03,
  STOP_GRAB_VALUE = 0x01000000,
  PIXEL_FORMAT_8BIT_VALUE = 0x01000801,
  PIXEL_FORMAT_12BIT_VALUE = 0x01000c01
};



uint32_t Read(asio::ip::udp::socket& sock, asio::ip::udp::resolver::iterator& it, EnAddrs addr)
{
   boost::array<char, 12> buff = {{ 0x42, 0x01, 0x00, 0x80, 0x00, 0x04 }};
   ((uint16_t*)&buff)[3] = htons(gnMsgNr++);
   ((uint32_t*)&buff)[2] = htonl(addr);
   sock.send_to(asio::buffer(buff), *it);

   boost::array<char, 256> recv_buf;
   asio::ip::udp::endpoint sender_endpoint;
   /*size_t len =*/ sock.receive_from(asio::buffer(recv_buf), sender_endpoint);

   return htonl(((uint32_t*)&recv_buf)[2]);
}

bool Write(asio::ip::udp::socket& sock, asio::ip::udp::resolver::iterator& it, EnAddrs addr, uint32_t nVal)
{
  boost::array<char, 16> buff = {{ 0x42, 0x01, 0x00, 0x82, 0x00, 0x08 }};
  ((uint16_t*)&buff)[3] = htons(gnMsgNr++);
  ((uint32_t*)&buff)[2] = htonl(addr);
  ((uint32_t*)&buff)[3] = htonl(nVal);
  sock.send_to(asio::buffer(buff), *it);

  boost::array<char, 256> recv_buf;
  asio::ip::udp::endpoint sender_endpoint;
  if(sock.receive_from(asio::buffer(recv_buf), sender_endpoint) != 12)
    return false;
  else
    return recv_buf[11] == 0x01;
}


asio::ip::address_v4 FindCam(asio::ip::udp::socket& sock, asio::ip::udp::resolver::iterator& it)
{
  boost::array<char, 8> buff = {{ 0x42, 0x01, 0x00, 0x02, 0x00, 0x00 }};
  ((uint16_t*)&buff)[3] = htons(gnMsgNr++);
  sock.send_to(asio::buffer(buff), *it);

  boost::array<char, 256> recv_buf;
  asio::ip::udp::endpoint sender_endpoint;
  /*size_t len =*/ sock.receive_from(asio::buffer(recv_buf), sender_endpoint);
  asio::ip::address_v4 addr(recv_buf[47] + ((uint32_t(recv_buf[46]))<<8) +
                            ((uint32_t(recv_buf[45]))<<16) + ((uint32_t(recv_buf[44]))<<24));
  return addr;
}

void Heartbeat(asio::ip::udp::socket* pSock, asio::ip::udp::resolver::iterator& it,
               asio::deadline_timer* pTimer)
{
  std::cout << "Bing! "
            << (Read(*pSock, it, ACCESS_CAM) == 0x3) << std::endl;

  if(gbHeartbeatRun == true)
  {
    pTimer->expires_at(pTimer->expires_at() + boost::posix_time::millisec(700));
    pTimer->async_wait(boost::bind(Heartbeat, pSock, it, pTimer));
  }
}

int main(int argc, char** argv)
{
  try
  {
    if(argc != 3)
      throw std::runtime_error(str(boost::format("Usage: %1% <ip> <port>") % argv[0]));

    asio::io_service service;
    asio::ip::udp::socket socket(service, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0));
    asio::ip::udp::resolver resolver(service);

    asio::ip::udp::resolver_query query(asio::ip::udp::v4(), argv[1], argv[2]);
    asio::ip::udp::resolver::iterator it = resolver.resolve(query);

    asio::ip::address_v4 camAddr = FindCam(socket, it);
    std::cout << "Cam Addr: " << camAddr.to_string() << std::endl;

    // format bool values as strings
    std::cout.setf (std::ios::boolalpha);

    std::cout << "Access acquired: "
              << Write(socket, it, ACCESS_CAM, ACCESS_CAM_VALUE) << std::endl;
    std::cout << "Set stream port: "
              << Write(socket, it, DATASTREAM_PORT, 12221) << std::endl;
    std::cout << "Set stream addr: "
              << Write(socket, it, DATASTREAM_ADDRESS,
                       asio::ip::address_v4::from_string("169.254.0.2").to_ulong()) << std::endl;
    std::cout << "Set pixel format: " << Write(socket, it, PIXEL_FORMAT, PIXEL_FORMAT_8BIT_VALUE) << std::endl;
    std::cout << "Pixel format: " << (Read(socket, it, PIXEL_FORMAT) == PIXEL_FORMAT_8BIT_VALUE ?
                                      "8bit" : "12bit") << std::endl;
    std::cout << "Start: "
              << Write(socket, it, START_GRAB, 0x0) << std::endl;

    // start heartbeat thread
    gbHeartbeatRun = true;
    asio::deadline_timer heartbeat(service, boost::posix_time::millisec(700));
    heartbeat.async_wait(boost::bind(Heartbeat, &socket, it, &heartbeat));
    boost::thread ioThread(boost::bind(&                                                                                                  asio::io_service::run, &service));

    // start data thread
    GvspReader reader(service, 12221);

    sleep(10);
    reader.Stop();
    gbHeartbeatRun = false;
    ioThread.join();

    std::cout << "Stop: "
              << Write(socket, it, STOP_GRAB, STOP_GRAB_VALUE) << std::endl;

    std::cout << "Waiting..." << std::flush;
    while(Read(socket, it, GRABBING_STOPPED) != STOP_GRAB_VALUE)
      std::cout << "." << std::flush;
    std::cout << " stopped" << std::endl;


  }
  catch(std::exception& e)
  {
    std::cout << "Exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}


