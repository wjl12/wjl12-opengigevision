#ifndef GENICAM_H
#define GENICAM_H

#include <exception>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/copy.hpp>
#include <zlib.h>
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
    std::vector<int8_t> data = m_gvcp.ReadBlock(GENICAM_ZIPFILEINFO_ADDRESS, 512);
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

    std::cout << "Size:" << data.size() << std::endl;
    std::ofstream os("out.zip", std::ios::binary);
    os.write((char*)(&data[0]), data.size());
    os.flush();

    boost::iostreams::filtering_istream in;
    in.push(boost::iostreams::zlib_decompressor());
    in.push(boost::iostreams::basic_array_source<char>((char*)(&data[0]), data.size()));

    std::vector<char> outData;
    boost::iostreams::copy(in, boost::iostreams::back_inserter(outData));

    return outData;
  }

  private:
  GvcpManager& m_gvcp;
};

#endif // GENICAM_H
