#include <iostream>
#include <exception>
#include <fstream>
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <vigra/stdimage.hxx>
#include <vigra/impex.hxx>

#include "GigEVision.h"
#include "GenICam.h"
#include "Gvcp.h"
#include "Gvsp.h"


int main(int argc, char** argv)
{
  try
  {
    if(argc != 3)
      throw std::runtime_error(str(boost::format("Usage: %1% <ip> <port>") % argv[0]));

    GigEVision vis;
    vis.Gvcp().Connect(argv[1], argv[2]);
    std::cout << "Cam Addr: " << vis.Gvcp().FindCam().to_string() << std::endl;
    vis.GenICam().ReadXmlFile();
    vis.GenICam().PrintNodes();

    // format bool values as strings
    std::cout.setf (std::ios::boolalpha);
    std::cout << "Access acquired: "
              << vis.Gvcp().Write("Cust::GevCCPReg", EXCLUSIVE_ACCESS) << std::endl;
    std::cout << "Set stream port: "
              << vis.Gvcp().Write("Std::GevSCPHostPort", 12221) << std::endl;
    std::cout << "Set stream addr: "
              << vis.Gvcp().Write("Std::GevSCDA",
                       boost::asio::ip::address_v4::from_string("169.254.0.2").to_ulong()) << std::endl;
    std::cout << "Set pixel format: " << vis.Gvcp().Write(PIXEL_FORMAT, PIXEL_FORMAT_8BIT_VALUE) << std::endl;
    std::cout << "Pixel format: " << (vis.Gvcp().Read(PIXEL_FORMAT) == PIXEL_FORMAT_8BIT_VALUE ?
                                      "8bit" : "12bit") << std::endl;
    std::cout << "Start: "
              << vis.Gvcp().Write("Cust::AcquisitionStartReg", 0x0) << std::endl;

    vis.Gvcp().StartHeartbeat();

    sleep(10);
    vis.Gvsp().Stop();
    vis.Gvcp().StopHeartbeat();

    std::cout << "Stop: " << vis.Gvcp().Write("Cust::AcquisitionStopReg", STOP_GRAB_VALUE) << std::endl;

    std::cout << "Waiting..." << std::flush;
    while(vis.Gvcp().Read(GRABBING_STOPPED) != STOP_GRAB_VALUE)
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


