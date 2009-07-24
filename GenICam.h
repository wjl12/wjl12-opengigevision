#ifndef GENICAM_H
#define GENICAM_H

#include <exception>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "Gvcp.h"

class GenICamManager
{
  public:
  GenICamManager(GvcpManager& gvcp):
      m_gvcp(gvcp)
  {

  }

  std::vector<char> ReadXmlFile()
  {
    std::vector<char> data = m_gvcp.ReadBlock(GENICAM_ZIPFILEINFO_ADDRESS, 512);
    std::string sTxt(data.begin(), data.end());

    std::vector<std::string> aParts;
    boost::algorithm::split(aParts, sTxt, boost::algorithm::is_any_of(";"));

    if(aParts.size() != 3)
      throw std::runtime_error("Error getting genicam file location on camera");

    int nAddr, nSize;
    std::stringstream ss;
    ss << std::hex << aParts[1];
    ss >> nAddr;
    ss.clear();
    ss << std::hex << aParts[2];
    ss >> nSize;

    return m_gvcp.ReadBlock(nAddr, nSize);
  }

  private:
  GvcpManager& m_gvcp;
};

#endif // GENICAM_H
