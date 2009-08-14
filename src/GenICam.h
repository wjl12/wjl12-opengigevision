#ifndef GENICAM_H
#define GENICAM_H

#include <string>
#include "GenApi/GenApi.h"

// forwarding
class GigEVision;

class GenICamManager
{
  public:
  GenICamManager(GigEVision& gev);

  void ReadXmlFile();

  uint32_t GetAddress(const std::string& sKey);

  void PrintNodes(bool bWithProperties=false) const;

  private:
  GigEVision& m_gev;
  GenApi::CNodeMapRef m_cam;
};

#endif // GENICAM_H
