#ifndef GIGEVISION_H
#define GIGEVISION_H

#include <memory>
#include <boost/asio.hpp>

// forwarding
class GenICamManager;
class GvcpManager;
class GvspManager;

class GigEVision
{
  public:
  GigEVision();

  GenICamManager& GenICam() { return *m_genicam; }
  GvcpManager& Gvcp() { return *m_gvcp; }
  GvspManager& Gvsp() { return *m_gvsp; }
  boost::asio::io_service& Service() { return m_service; }

  private:
  boost::asio::io_service m_service;
  std::auto_ptr<GenICamManager> m_genicam;
  std::auto_ptr<GvcpManager> m_gvcp;
  std::auto_ptr<GvspManager> m_gvsp;
};


#endif // GIGEVISION_H
