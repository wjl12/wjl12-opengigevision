#ifndef GVSP_H
#define GVSP_H

#include <arpa/inet.h>
#include <vigra/stdimage.hxx>
#include <asio.hpp>

class GvspManager
{
  public:
    GvspManager(asio::io_service& service, int nPort):
        m_socket(service, asio::ip::udp::endpoint(asio::ip::udp::v4(), nPort)),
        m_nCurrPartOfFrame(0),
        m_nImg(0),
        m_bRun(false)
    {
      StartReceive();
    }

    ~GvspManager()
    {
      Stop();
      // TODO: socket wait or close
    }

    void Stop() { m_bRun = false; }

  private:
    void StartReceive()
    {
      m_bRun = true;
      asio::ip::udp::endpoint ep;
      m_socket.async_receive_from(asio::buffer(m_buff), ep,
                                  boost::bind(&GvspManager::RecHandler, this,
                                              asio::placeholders::error,
                                              asio::placeholders::bytes_transferred));
    }

    void RecHandler(const asio::error_code& error, std::size_t nBytes)
    {
      if (!error || error == asio::error::message_size)
      {
        uint16_t nPartOfFrame = ntohs(((uint16_t*)&m_buff)[3]);
       // std::cout << "Got data:" << nBytes << " bytes\tFrame# " << ntohl(((uint32_t*)&m_buff)[0])
       //           << "\tPart of frame# " << nPartOfFrame;

        if(m_buff[4] == 0x01)
        {
          m_nCurrPartOfFrame = nPartOfFrame;
          assert(m_nCurrPartOfFrame == 0);
          uint16_t nWidth = ntohs(((uint16_t*)&m_buff)[13]);
          uint16_t nHeight = ntohs(((uint16_t*)&m_buff)[15]);
          std::cout << "\tHeader width: " << nWidth << "\theight: " << nHeight << std::endl;
          m_img.resize(nWidth, nHeight);
        }
        else if(m_buff[4] == 0x03)
        {
          if(nPartOfFrame != m_nCurrPartOfFrame+1)
            std::cout << "\n***Missing Package***" << std::endl;

          m_nCurrPartOfFrame = nPartOfFrame;
          int nPayloadSize = nBytes-8;
          std::copy(m_buff.begin()+8, m_buff.begin() + nPayloadSize, m_img.begin()+nPartOfFrame*nPayloadSize);
//          std::cout << "\tData size" << nPayloadSize << std::endl;
        }
        else if(m_buff[4] == 0x02)
        {
          std::cout << "\tFooter" << std::endl;
          vigra::exportImage(vigra::srcImageRange(m_img), vigra::ImageExportInfo(
              str(boost::format("%|03|.png") % m_nImg++).c_str()));
        }
        else
          std::cout << "\tUnknown frame" << std::endl;

        if(m_bRun)
          StartReceive();
      }
      else
        std::cerr << "Error receiving stream data" << std::endl;
    }

    asio::ip::udp::socket m_socket;
    boost::array<uint8_t, 2048> m_buff;
    vigra::BImage m_img;
    int m_nCurrPartOfFrame;
    int m_nImg;
    bool m_bRun;
};



#endif // GVSP_H
