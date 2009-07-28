#ifndef GENICAM_H
#define GENICAM_H

#include <exception>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <zip.h>
#include "Gvcp.h"

class GenICamManager
{
  public:
  GenICamManager(GvcpManager& gvcp):
      m_gvcp(gvcp)
  {

  }

  std::vector<uint8_t> ReadXmlFile()
  {
    std::vector<uint8_t> data = m_gvcp.ReadBlock(GENICAM_ZIPFILEINFO_ADDRESS, 512);
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

    data = m_gvcp.ReadBlock(nAddr, nSize);

    // TODO: find a way to decompress the zip-file in menory without writing the file to disk
    // TODO: use a platform independent path
    std::string sFilename("/tmp/genicam.zip");
    std::vector<uint8_t> retData;

    try
    {
      std::ofstream os(sFilename.c_str(), std::ios::binary);
      os.write((char*)(&data[0]), data.size());
      os.flush();

      struct zip* zz = zip_open(sFilename.c_str(), 0, 0);
      if(zz == 0)
        throw std::runtime_error("Error opening genicam zipfile");

      struct zip_file* zf = zip_fopen_index(zz, 0, 0);
      if(zf == 0)
        throw std::runtime_error("Error opening file in zip archive");

      boost::array<uint8_t, 1024> buff;

      int nRead = 0;
      while((nRead = zip_fread(zf, buff.data(), buff.size())) != 0)
        std::copy(buff.begin(), buff.begin()+nRead, std::back_inserter(retData));

      // clean up file from disk
      boost::filesystem::remove(sFilename);
    }
    catch(std::exception& e)
    {
      boost::filesystem::remove(sFilename);
      throw e;
    }

    return retData;
  }

  private:
  GvcpManager& m_gvcp;
};

#endif // GENICAM_H
