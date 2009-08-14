#ifndef GVSP_H
#define GVSP_H

#include <boost/asio.hpp>
#include <boost/gil/gil_all.hpp>

class GigEVision;

class GvspManager
{
  public:
    GvspManager(GigEVision& gev, int nPort);

    ~GvspManager();

    void Stop();

  private:
    void StartReceive();

    void RecHandler(const boost::system::error_code& error, std::size_t nBytes);

    GigEVision& m_gev;
    boost::asio::ip::udp::socket m_socket;
    boost::array<uint8_t, 2048> m_buff;
    boost::gil::gray8_image_t m_img;
    int m_nCurrPartOfFrame;
    int m_nImg;
    bool m_bRun;
};



#endif // GVSP_H
