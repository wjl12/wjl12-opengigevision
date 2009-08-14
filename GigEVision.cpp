#include "GenICam.h"
#include "Gvcp.h"
#include "Gvsp.h"

#include "GigEVision.h"

GigEVision::GigEVision():
      m_genicam(new GenICamManager(*this)),
      m_gvcp(new GvcpManager(*this)),
      m_gvsp(new GvspManager(*this, 12221))
{ }


