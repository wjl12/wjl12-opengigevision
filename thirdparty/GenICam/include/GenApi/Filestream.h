//-----------------------------------------------------------------------------
//  (c) 2007 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Thies Moeller
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/Filestream.h,v 1.5.2.6 2009/01/14 14:38:42 hartmut_nebelung Exp $
//
//  License: This file is published under the license of the EMVA GenICam  Standard Group.
//  A text file describing the legal terms is included in  your installation as 'GenICam_license.pdf'.
//  If for some reason you are missing  this file please contact the EMVA or visit the website
//  (http://www.genicam.org) for a full copy.
//
//  THIS SOFTWARE IS PROVIDED BY THE EMVA GENICAM STANDARD GROUP "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE EMVA GENICAM STANDARD  GROUP
//  OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,  SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT  LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,  DATA, OR PROFITS;
//  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY  THEORY OF LIABILITY,
//  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE  OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
/*!
\file
\brief    Definition of ODevFileStream and IDevFileStream
*/

#ifndef GENAPI_FILESTREAM_H_
#define GENAPI_FILESTREAM_H_

#include <iostream>
#include <streambuf>
#include <iomanip>
#include <iosfwd>

#include <GenApi/Types.h>
#include <GenApi/Pointer.h>
#include <GenApi/NodeMapRef.h>

#include <Base/GCUtilities.h>
#include <algorithm>

#ifdef _MSC_VER
#pragma push_macro("min")
#undef min
#endif

namespace GenICam
{

    /*!
    * @brief
    *	Align an integer
    *
    * @param v
    * value to align
    *
    * @param i
    * alignement
    *
    * @returns
    * v aligned at i
    *
    */
    inline
        int64_t Align( int64_t v, const int64_t i ){
            if (i==1) {
                return v;
            } else if (i>1) {
                int64_t r = (v+(i-1))/i;
                return r*i;
            } else {
                throw LOGICAL_ERROR_EXCEPTION( "Unexpected increment %d", i );
            }
    };

    /*!
    * @brief
    * Adapter between the std::iostreambuf and the SFNC Features representing the device filesystem
    *
    * The adapter assumes, that the features provide stdio fileaccess
    * compatible semantic
    *
    */
    class FileProtocolAdapter {
    public:
        /*!
        * @brief
        * Constructor
        *
        */
        FileProtocolAdapter()
            : m_pInterface(0)
#ifdef LOG4C
            , m_pMiscLog(NULL)
#endif
        {};
/*!
        * @brief
        * attach file protocol adapter to nodemap
        *
        * @param pInterface
        * NodeMap of the device to which the FileProtocolAdapter is attached
        *
        * @return true if attach was successful, false if not
        *
        */
        bool attach(GenApi::INodeMap * pInterface ){
            
            m_pInterface = pInterface;

            #ifdef LOG4C
                // init logging
                GenICam::gcstring Name( "FileProtocolAdapter" );

                GenICam::gcstring DeviceName = "NO_DEVICE";
                if (m_pInterface){
                    DeviceName = pInterface->GetDeviceName();
                }

                m_pMiscLog = &( GenICam::CLog::GetLogger("GenApi." + DeviceName + ".Misc." + Name));
            #endif

            // fetch all required nodes
            bool success = true;

            m_ptrFileSelector          = m_pInterface->GetNode("FileSelector");
            if( !m_ptrFileSelector.IsValid() ){
                success &= false;
                #ifdef LOG4C
                    m_pMiscLog->warn( "FileSelector invalid\n");
                #endif
            }

            m_ptrFileOperationSelector = m_pInterface->GetNode("FileOperationSelector");
            if( !m_ptrFileOperationSelector.IsValid() ){
                success &= false;
                #ifdef LOG4C
                    m_pMiscLog->warn( "FileOperationSelector invalid\n");
                #endif
            } else {
                // missing operations trigger exceptions in the adapter code
            }

            m_ptrFileOperationExecute         = m_pInterface->GetNode("FileOperationExecute");
            if( !m_ptrFileOperationExecute.IsValid() ){
                success &= false;
                #ifdef LOG4C
                    m_pMiscLog->warn( "FileOperationExecute invalid\n");
                #endif
            } 

            m_ptrFileOpenMode          = m_pInterface->GetNode("FileOpenMode");
            if( !m_ptrFileOpenMode.IsValid() ){
                success &= false;
                #ifdef LOG4C
                    m_pMiscLog->warn( "FileOpenMode invalid\n");
                #endif
            }

            m_ptrFileAccessOffset      = m_pInterface->GetNode("FileAccessOffset");
            if( !m_ptrFileAccessOffset.IsValid() ){
                success &= false;
                #ifdef LOG4C
                    m_pMiscLog->warn( "FileAccessOffset invalid\n");
                #endif
            }

            m_ptrFileAccessLength      = m_pInterface->GetNode("FileAccessLength");
            if( !m_ptrFileAccessLength.IsValid() ){
                success &= false;
                #ifdef LOG4C
                    m_pMiscLog->warn( "FileAccessLength invalid\n");
                #endif
            }

            m_ptrFileAccessBuffer            = m_pInterface->GetNode("FileAccessBuffer");
            if( !m_ptrFileAccessBuffer.IsValid() ){
                success &= false;
                #ifdef LOG4C
                    m_pMiscLog->warn( "FileAccessBuffer invalid\n");
                #endif
            }

            m_ptrFileOperationStatus = m_pInterface->GetNode("FileOperationStatus");
            if( !m_ptrFileOperationStatus.IsValid() ){
                success &= false;
                #ifdef LOG4C
                    m_pMiscLog->warn( "FileOperationStatus invalid\n");
                #endif
            } else {
                // missing success code trigger exceptions in the adapter code
            }

            m_ptrFileOperationResult            = m_pInterface->GetNode("FileOperationResult");
            if( !m_ptrFileOperationResult.IsValid() ){
                success &= false;
                #ifdef LOG4C
                    m_pMiscLog->warn( "FileOperationResult invalid\n");
                #endif
            }
            return success;
        };



        /*!
        * @brief
        * open a file on the device
        *
        * @param pFileName
        * filename of the file to open. The filename must exist in the Enumeration FileSelector
        *
        * @param mode
        * mode to open the file. The mode must exist in the Enunmeration FileOpenMode
        *
        * @returns
        * true on success, false on error
        *
        */
        bool openFile(const char * pFileName, std::ios_base::openmode mode){

            if ( ! m_ptrFileSelector.IsValid() ){
            #ifdef LOG4C
                m_pMiscLog->info("IsDone...");
            #endif				
                return false;
            }
            m_ptrFileSelector->FromString(pFileName);

            if (mode & (std::ios_base::out | std::ios_base::trunc)){
                m_ptrFileOpenMode->FromString("Write");
            }
            else if (mode & (std::ios_base::in)){
                m_ptrFileOpenMode->FromString("Read");
            }
            else{
                return false;
            }

            m_ptrFileOperationSelector->FromString("Open");
            m_ptrFileOperationExecute->Execute();
            
            return  m_ptrFileOperationStatus->ToString() == "Success";
        }


        /*!
        * @brief
        * close a file on the device
        *
        * @param pFileName
        * filename of the file to open. The filename must exist in the Enumeration FileSelector
        *
        * @returns
        * true on success, false on error
        */
        bool closeFile(const char * pFileName){
            m_ptrFileSelector->FromString(pFileName);
            m_ptrFileOperationSelector->FromString("Close");
            m_ptrFileOperationExecute->Execute();
            return m_ptrFileOperationStatus->ToString() == "Success";
        }

        /*!
        * @brief
        * writes data into a file.
        *
        * @param buf
        * source buffer
        *
        * @param offs
        * offset into the device file
        *
        * @param len
        * count of bytes to write
        *
        * @param pFileName
        * filename of the file to write into The filename must exist in the Enumeration FileSelector
        *
        * @returns
        * count of bytes written
        *
        */
        std::streamsize write(const char * buf, int64_t offs, int64_t len, const char * pFileName)
        {
            m_ptrFileSelector->FromString(pFileName);
            m_ptrFileOperationSelector->FromString("Write");

            const std::streamsize maxWriteLen((std::streamsize)(m_ptrFileAccessBuffer->GetLength()));

            std::streamsize bytesWritten = 0;
            while (bytesWritten < len){
                // copy streamdata to xchange buffer
                const int64_t
                                    remain(len - bytesWritten),
                                    writeSize(remain <= maxWriteLen ? remain : maxWriteLen);


                // offset
                if ( offs + bytesWritten > m_ptrFileAccessOffset->GetMax()){
                    // we cannot move the outbuffer any further
                    break;
                }
                m_ptrFileAccessOffset->SetValue(offs + bytesWritten);

                // writesize
                m_ptrFileAccessLength->SetValue(writeSize);

                // set the buffer
                m_ptrFileAccessBuffer->Set((const uint8_t *)buf+bytesWritten,Align(writeSize,4));

                // trigger Command
                m_ptrFileOperationExecute->Execute();

                bytesWritten += INTEGRAL_CAST<std::streamsize>(m_ptrFileOperationResult->GetValue());

                if (m_ptrFileOperationStatus->ToString() != "Success"){
                    // no more space on device
                    break;
                }
            }
            return bytesWritten;
        };

        /*!
        * @brief
        * read data from the device into a buffer
        *
        * @param buf
        * target buffer
        *
        * @param offs
        * offset in the device file to read from
        *
        * @param len
        * count of bytes to read
        *
        * @param pFileName
        * filename of the file to write into The filename must exist in the Enumeration FileSelector
        *
        * @returns
        * count of bytes successfully read
        *
        */
        std::streamsize read(char * buf, int64_t offs, std::streamsize len, const char * pFileName)
        {

            m_ptrFileSelector->FromString(pFileName);
            m_ptrFileOperationSelector->FromString("Read");
            const std::streamsize maxReadLen = INTEGRAL_CAST<std::streamsize>(m_ptrFileAccessBuffer->GetLength());

            std::streamsize bytesRead = 0;
            while (bytesRead < len){
                // copy xchange buffer to streamdata
                const std::streamsize readSize = std::min(len - bytesRead, maxReadLen);
                // offset
                if ( offs + bytesRead > m_ptrFileAccessOffset->GetMax()){
                    // we cannot move the inbuffer any further
                    break;
                }
                m_ptrFileAccessOffset->SetValue(offs + bytesRead);

                // readsize
                m_ptrFileAccessLength->SetValue(readSize);

                // fetch file data into xchange buffer
                m_ptrFileOperationExecute->Execute();

                std::streamsize result = INTEGRAL_CAST<std::streamsize>(m_ptrFileOperationResult->GetValue());
                m_ptrFileAccessBuffer->Get((uint8_t *)buf + bytesRead,readSize);

                bytesRead += result;
                if (m_ptrFileOperationStatus->ToString() != "Success"){
                    // reached end of file
                    break;
                }
            }
            return bytesRead;

        };

        /*!
        * @brief
        * fetch max FileAccessBuffer length for a file
        *
        * @param pFileName
        * filename of the file to open. The filename must exist in the Enumeration FileSelector
        *
        * @param mode
        * mode to open the file. The mode must exist in the Enunmeration FileOpenMode
        *
        * @returns
        * max length of FileAccessBuffer in the given mode on the given file
        *
        */
        int64_t getBufSize(const char * pFileName, std::ios_base::openmode mode){

            m_ptrFileSelector->FromString(pFileName);

            if (mode & (std::ios_base::out | std::ios_base::trunc)){
                m_ptrFileOperationSelector->FromString("Write");
                return m_ptrFileAccessBuffer->GetLength();
            }
            else if (mode & (std::ios_base::in)){
                m_ptrFileOperationSelector->FromString("Read");
                return m_ptrFileAccessBuffer->GetLength();
            }
            else{
                return 0;
            }
        }
    private:
        // the device nodemap
        GenApi::INodeMap * m_pInterface;

        GenApi::CEnumerationPtr m_ptrFileSelector;
        GenApi::CEnumerationPtr m_ptrFileOperationSelector;
        GenApi::CCommandPtr m_ptrFileOperationExecute;
        GenApi::CEnumerationPtr m_ptrFileOpenMode;
        GenApi::CIntegerPtr m_ptrFileAccessOffset;
        GenApi::CIntegerPtr m_ptrFileAccessLength;
        GenApi::CRegisterPtr m_ptrFileAccessBuffer;
        GenApi::CEnumerationPtr m_ptrFileOperationStatus;
        GenApi::CIntegerPtr m_ptrFileOperationResult;

    private:
#ifdef LOG4C
        //! Logger for messages concerning miscellaneoud access which does not fit to the other categories
        log4cpp::Category *m_pMiscLog;
#endif


    };


    template<typename CharType, typename Traits> class IDevFileStreamBuf
        : public std::basic_streambuf<CharType, Traits> {

            typedef Traits traits_type;
            typedef typename Traits::int_type int_type;
            typedef typename Traits::char_type char_type;
            typedef IDevFileStreamBuf<CharType, Traits> filebuf_type;

            // GET next ptr
            using std::basic_streambuf<CharType, Traits>::gptr;
            // GET end ptr
            using std::basic_streambuf<CharType, Traits>::egptr;
            // GET begin ptr
            using std::basic_streambuf<CharType, Traits>::eback;
            // increment next pointer
            using std::basic_streambuf<CharType, Traits>::gbump;
            // set buffer info
            using std::basic_streambuf<CharType, Traits>::setg;

    public:
        IDevFileStreamBuf()
            : m_file(0), m_pAdapter(0), m_fpos(0) {
                // This already handled by the base class constructor, right?
        // std::basic_streambuf<CharType, Traits>::_Init();		
        };


        ~IDevFileStreamBuf(){
            this->close();
        }

        filebuf_type *open(GenApi::INodeMap * pInterface, const char * pFileName, std::ios_base::openmode mode = std::ios_base::in ) {
            // get file protocol adapter
            m_pAdapter = new FileProtocolAdapter();

            // open file via Adapter
            if (!m_pAdapter || !m_pAdapter->attach(pInterface)){
                delete m_pAdapter;
                m_pAdapter = 0;
                return 0;
            }

            // open file via Adapter
            if (!(m_pAdapter->openFile(pFileName, mode ))){
                delete m_pAdapter;
                m_pAdapter = 0;
                return 0;
            }

            m_file = pFileName;
            // alocate buffer according to fileinfo
            m_BufSize = (std::streamsize)m_pAdapter->getBufSize(m_file,mode);
            m_pBuffer = new char_type[m_BufSize / sizeof(char_type)];

            // setg(buffer+pbSize, buffer+pbSize, buffer+pbSize);
            setg(m_pBuffer, m_pBuffer + m_BufSize,m_pBuffer + m_BufSize);

        #ifdef _MSC_VER
        // is this reasonable?
            std::basic_streambuf<CharType, Traits>::_Init();
        #endif

            return this;
        }



        bool
            is_open() const
        {
            return m_pAdapter != 0;
        }

        filebuf_type *close() {
            filebuf_type * ret = 0;
            if (this->is_open()) {
                // close file
                if(m_pAdapter->closeFile(m_file)){
                    // no error
                    ret = this;
                }
                delete m_pAdapter;
                m_pAdapter = 0;
                // buffer
                delete[] m_pBuffer;
                m_pBuffer = 0;
            }
            return ret;
        }


    protected:
        int_type underflow() {
            if (gptr() < egptr() )
                return traits_type::to_int_type(*gptr());

            if (buffer_in() < 0)
                return traits_type::eof();
            else
                return traits_type::to_int_type(*gptr());

        }

        int_type pbackfail(int_type c) {
            if (gptr() != eback() || eback()<gptr()) {
                gbump(-1);
                if (!traits_type::eq_int_type(c, traits_type::eof() ) )
                    *(gptr()) = static_cast<char_type>(traits_type::not_eof(c));
                return traits_type::not_eof(c);
            } else
                return traits_type::eof();
        }

    private:
        char_type * m_pBuffer;
        std::streamsize m_BufSize;

        const char * m_file;
        FileProtocolAdapter * m_pAdapter;
        int64_t m_fpos;



        int buffer_in() {

            std::streamsize retval = m_pAdapter->read(m_pBuffer , m_fpos, m_BufSize,m_file);

            if (retval <= 0) {
                setg(0, 0, 0);
                return -1;
            } else {
                setg(m_pBuffer, m_pBuffer , m_pBuffer + retval);
                m_fpos += retval;
                return INTEGRAL_CAST2<int,std::streamsize>(retval);
            }
        }


        // prohibit copying and assignment
        IDevFileStreamBuf(const IDevFileStreamBuf&);
        IDevFileStreamBuf& operator=(const IDevFileStreamBuf&);
    };


    template<typename CharType, typename Traits> class ODevFileStreamBuf
        : public std::basic_streambuf<CharType, Traits> {
            typedef Traits traits_type;

            typedef typename Traits::int_type int_type;
            typedef typename Traits::char_type char_type;
            typedef typename Traits::pos_type pos_type;
            typedef typename Traits::off_type off_type;

            typedef ODevFileStreamBuf<CharType, Traits> filebuf_type;


            // PUT begin
            using std::basic_streambuf<CharType, Traits>::pbase;
            // PUT next
            using std::basic_streambuf<CharType, Traits>::pptr;
            // PUT end
            using std::basic_streambuf<CharType, Traits>::epptr;
            // increment next pointer
            using std::basic_streambuf<CharType, Traits>::pbump;

    public:
        ODevFileStreamBuf()
            : m_file(0), m_pAdapter(0), m_fpos(0) {
        }

        ~ODevFileStreamBuf() {
            this->close();
        }

        filebuf_type *open(GenApi::INodeMap * pInterface, const char * pFileName, std::ios_base::openmode mode) {

            // create Genicam Access
            m_pAdapter = new FileProtocolAdapter();

            // attach to nodemap
            if (!m_pAdapter || !m_pAdapter->attach(pInterface)){
                delete m_pAdapter;
                m_pAdapter = 0;
                return 0;
            }
                        

            // open file via Adapter
            if (!(m_pAdapter->openFile(pFileName, mode ))){
                delete m_pAdapter;
                m_pAdapter = 0;
                return 0;
            }

            m_file = pFileName;
            // allocate buffer according to fileinfo
            const int64_t bufSize = m_pAdapter->getBufSize(m_file,mode);
            m_pBuffer = new char_type[INTEGRAL_CAST<size_t>(bufSize) / sizeof(char_type)];
            setp(m_pBuffer, m_pBuffer + bufSize);

            return this;
        }

        bool
            is_open() const {
                return m_pAdapter != 0;
        }

        filebuf_type *close() {
            filebuf_type * ret = 0;
            bool syncFailed = false;
            if (this->is_open()) {
                if (sync()){
                    syncFailed = true;
                };
                // close file
                if(m_pAdapter->closeFile(m_file)){
                    // no error
                    if ( syncFailed ){
                        ret = 0;
                    } else {
                    ret = this;
                }
                }
                delete m_pAdapter;
                m_pAdapter = 0;
                // buffer
                delete[] m_pBuffer;
                m_pBuffer = 0;
            }
            return ret;
        }

    protected:
        std::streamsize xsputn(const char_type * s, std::streamsize n) {
            if (n < epptr() - pptr() ) {
                memcpy(pptr(), s, n * sizeof(char_type));
                pbump( INTEGRAL_CAST2<int>(n) );
                return n;
            } else {
                for (std::streamsize i = 0; i<n; ++i ) {
                    if (traits_type::eq_int_type(sputc(s[i]), traits_type::eof()))
                        return i;
                }
                return n;
            }

        }
        int_type overflow(int_type c = traits_type::eof()) {
            if (buffer_out() < 0) {
                return traits_type::eof();
            } else {
                if (!traits_type::eq_int_type (c, traits_type::eof() ) )
                    return sputc(static_cast<char_type>(c));
                else
                    return traits_type::not_eof(c);
            }

        }
        int sync() {
            return INTEGRAL_CAST<int>(buffer_out());
        }

    private:
        char_type * m_pBuffer; // buffer[bufSize];
        const char * m_file;
        FileProtocolAdapter * m_pAdapter;
        int64_t m_fpos;

        int64_t buffer_out() {
            int64_t cnt = pptr() - pbase();

            int64_t retval;
            int64_t res = m_pAdapter->write(m_pBuffer, m_fpos, cnt, m_file);

            if (res != cnt) {
                retval = -1;
            } else {
                retval = 0;
            }
            m_fpos += res;

            pbump(- INTEGRAL_CAST<int>(cnt));
            return retval;
        }

        // prohibit copying assignment
        ODevFileStreamBuf(const ODevFileStreamBuf&);
        ODevFileStreamBuf & operator =(const ODevFileStreamBuf&);
    };

    template<typename CharType, typename Traits> class ODevFileStreamBase
        : public std::basic_ostream<CharType, Traits> {
    public:
        // Non-standard types:
        typedef ODevFileStreamBuf<CharType, Traits> filebuf_type;
        typedef std::basic_ios<CharType, Traits> ios_type;
        typedef std::basic_ostream<CharType, Traits> ostream_type;

    private:
        filebuf_type m_streambuf;
    public:

#if defined (_MSC_VER)
        ODevFileStreamBase()
            : ostream_type(std::_Noinit), m_streambuf() {
                this->init(&m_streambuf);
        }

        ODevFileStreamBase(GenApi::INodeMap * pInterface, const char * pFileName, std::ios_base::openmode mode = std::ios_base::out|std::ios_base::trunc)
            : ostream_type(std::_Noinit), m_streambuf() {
                this->init(&m_streambuf);
                this->open(pInterface, pFileName, mode);
        }
        ;

#elif defined (__GNUC__)
        ODevFileStreamBase()
            : ostream_type(), m_streambuf() {
                this->init(&m_streambuf);
        }

        ODevFileStreamBase(GenApi::INodeMap * pInterface, const char * pFileName, std::ios_base::openmode mode = std::ios_base::out|std::ios_base::trunc)
            : ostream_type(), m_streambuf() {
                this->init(&m_streambuf);
                this->open(pInterface, pFileName, mode);
        }
        ;


#else
#   error Unknown C++ library
#endif                



        filebuf_type *rdbuf() const {
            return const_cast<filebuf_type*>(&m_streambuf);
        }

        /* bool is_open() {
            return m_streambuf.is_open();
        } */

        bool is_open() const {
            return m_streambuf.is_open();
        }

        /**
        *  @brief  
        *	Open file on device	in write mode
        *	
        *  @param  pInterface  NodeMap of the device to which the FileProtocolAdapter is attached
        *  @param  pFileName  Name of the file to open
        *  @param  mode open mode
        *
        */
        void open(GenApi::INodeMap * pInterface, const char * pFileName,
            std::ios_base::openmode mode = std::ios_base::out | std::ios_base::trunc) {
                if (!m_streambuf.open(pInterface,pFileName, mode)){
                    this->setstate(std::ios_base::failbit);
                }
                else{
                    this->clear();
                }
        }

        /**
        *  @brief  Close the file on device
        *
        */
        void close() {
            if (!m_streambuf.close())
                this->setstate(std::ios_base::failbit);
        }
    };

    template<typename CharType, typename Traits> class IDevFileStreamBase
        : public std::basic_istream<CharType, Traits> {
    public:

        // Non-standard types:
        typedef IDevFileStreamBuf<CharType, Traits> filebuf_type;
        typedef std::basic_ios<CharType, Traits> ios_type;
        typedef std::basic_istream<CharType, Traits> istream_type;

    private:
        filebuf_type m_streambuf;
    public:

#if defined (_MSC_VER)
        IDevFileStreamBase()
            : istream_type(std::_Noinit), m_streambuf() {
                this->init(&m_streambuf);
        }

        IDevFileStreamBase(GenApi::INodeMap * pInterface, const char * pFileName,
            std::ios_base::openmode mode = std::ios_base::in)
            : istream_type(std::_Noinit), m_streambuf() {
                this->init(&m_streambuf);
                this->open(pInterface, pFileName, mode);
        }
        ;

#elif defined (__GNUC__)
        IDevFileStreamBase()
            : istream_type(), m_streambuf() {
                this->init(&m_streambuf);
        }

        IDevFileStreamBase(GenApi::INodeMap * pInterface, const char * pFileName,
            std::ios_base::openmode mode = std::ios_base::in)
            : istream_type(), m_streambuf() {
                this->init(&m_streambuf);
                this->open(pInterface, pFileName, mode);
        }
        ;

#else
#   error Unknown C++ library
#endif                


        filebuf_type *rdbuf() const {
            return const_cast<filebuf_type*>(&m_streambuf);
        }

        /* bool is_open() {
            return m_streambuf.is_open();
        } */

        bool is_open() const {
            return m_streambuf.is_open();
        }

        /* @brief
        *	Open file on device	in write mode
        *	
        *  @param  pInterface  NodeMap of the device to which the FileProtocolAdapter is attached
        *  @param  pFileName  Name of the file to open
        *  @param  mode open mode
        */
        void open(GenApi::INodeMap * pInterface, const char * pFileName,
            std::ios_base::openmode mode = std::ios_base::in) {
                if (!m_streambuf.open(pInterface,pFileName, mode))
                    this->setstate(std::ios_base::failbit);
                else
                    this->clear();
        }

        /**
        *  @brief  Close the file on the device
        */
        void close() {
            if (!m_streambuf.close())
                this->setstate(std::ios_base::failbit);
        }
    };

    typedef ODevFileStreamBase<char, std::char_traits<char> > ODevFileStream;
    typedef IDevFileStreamBase<char, std::char_traits<char> > IDevFileStream;
}

#ifdef _MSC_VER
#pragma pop_macro("min")
#endif

#endif /*GENAPI_FILESTREAM_H_*/
