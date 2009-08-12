#ifndef GENICAM_H
#define GENICAM_H

#include <cstdlib>
#include <exception>
#include <sstream>
#include <iostream>
#include <functional>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <zip.h>

#include "GenApi/GenApi.h"
#include "Gvcp.h"

class GenICamManager
{
  public:
  GenICamManager(GvcpManager& gvcp):
      m_gvcp(gvcp)
  {

  }

  void ReadXmlFile()
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
    std::string retData;

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

    setenv("GENICAM_ROOT_V1_1", GENICAM_ROOT_PATH, true);
    m_cam._LoadXMLFromString(retData.c_str());
  }

  uint32_t GetAddress(const std::string& sKey)
  {
    GenApi::INode* pN = m_cam._GetNode(sKey.c_str());
    uint32_t nAddr = 0;
    if(pN != 0)
    {
      GenICam::gcstring val, att;
      if(pN->GetProperty("Address", val, att))
      {
        nAddr = boost::lexical_cast<uint32_t>(val);
      }
      if(pN->GetProperty("pAddress", val, att) && val.length() > 0)
      {
        std::string sTmp(val);
        std::vector<std::string> parts;
        boost::algorithm::split(parts, sTmp, boost::algorithm::is_any_of("\t"));
        for(size_t i=0; i<parts.size(); ++i)
        {
          GenApi::INode* pAddrNode = m_cam._GetNode(parts[i].c_str());
          assert(pAddrNode);
          GenICam::gcstring sAddr, sAtt;
          assert(pAddrNode->GetProperty("Value", sAddr, sAtt));
          nAddr += boost::lexical_cast<uint32_t>(sAddr);
        }
      }
    }
    else
      throw std::runtime_error(str(boost::format("Can't find address for '%1%'") % sKey));

    return nAddr;
  }

  void PrintNodes() const
  {
    GenApi::NodeList_t nodes;
    m_cam._GetNodes(nodes);

    for(GenApi::NodeList_t::const_iterator it = nodes.begin(); it!=nodes.end(); ++it)
    {
      std::cout << "Node: " << (*it)->GetName(true) << std::endl;
      GenICam::gcstring_vector props;
      (*it)->GetPropertyNames(props);
      for(GenICam::gcstring_vector::const_iterator pit = props.begin(); pit!=props.end(); ++pit)
        std::cout << "\tProp: " << *pit << std::endl;
    }
  }

  private:
  GvcpManager& m_gvcp;
  GenApi::CNodeMapRef m_cam;
};

#endif // GENICAM_H
