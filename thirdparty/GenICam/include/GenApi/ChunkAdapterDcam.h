//-----------------------------------------------------------------------------
//  (c) 2004 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  GenApi
//    Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/ChunkAdapterDcam.h,v 1.7.2.2 2008/12/08 14:40:19 hartmut_nebelung Exp $
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
/**
\file
\brief    Declaration of the CChunkAdapterDcam class.
*/

#ifndef GENAPI_CHUNKADAPTERDCAM_H
#define GENAPI_CHUNKADAPTERDCAM_H

#include "ChunkAdapter.h"
#include "impl/Compatibility.h"

namespace GenApi
{
    // set the value of a GUID
    #define SET_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        name.Data1 = l, name.Data2 = w1, name.Data3 = w2, \
        name.Data4[0] = b1, name.Data4[1] = b2, name.Data4[2] = b3, name.Data4[3] = b4, \
        name.Data4[4] = b5, name.Data4[5] = b6, name.Data4[6] = b7, name.Data4[7] = b8

    // make sure everything is properly packed
    #pragma pack(push, 1) 

    typedef struct DCAM_CHUNK_TRAILER
    {
        PACK_STRUCT_FIELD( GUID        ChunkID );
        PACK_STRUCT_FIELD( uint32_t ChunkLength );
        PACK_STRUCT_FIELD( uint32_t InverseChunkLength );
    } PACK_STRUCT_STRUCT DCAM_CHUNK_TRAILER;

    typedef struct DCAM_CHECKSUM
    {
        PACK_STRUCT_FIELD( uint32_t CRCChecksum );
    } PACK_STRUCT_STRUCT DCAM_CHECKSUM;

    // restore the previous packing
    #pragma pack(pop) 
    /* ------------------------------------------- */

    //! Connects a chunked DCAM buffer to a node map
    class GENAPI_DECL CChunkAdapterDcam : public  CChunkAdapter
    {

    public:
        //! Constructor
        CChunkAdapterDcam(INodeMap* pNodeMap = NULL);

        //! Destructor
        virtual ~CChunkAdapterDcam();

        //! Checks if a buffer contains chunks in a known format
        virtual bool CheckBufferLayout(uint8_t *pBuffer, int64_t BufferLength);

        //! Attaches a buffer to the matching ChunkPort
        virtual void AttachBuffer(uint8_t *pBuffer, int64_t BufferLength, AttachStatistics_t *pAttachStatistics = NULL);

        //! Checks if buffer has a CRC attached
        bool HasCRC(uint8_t *pBuffer, int64_t BufferLength);

        //! Checks CRC sum of buffer
        bool CheckCRC(uint8_t *pBuffer, int64_t BufferLength);

    protected:
        //! Checks the CRC of a buffer
        uint32_t CRC16(const unsigned char *pData, uint32_t nbyLength );

        //! Checks the CRC of a buffer
        bool CheckBuffer(const unsigned char* pData, uint32_t nbyLength );

    };

}

#endif // GENAPI_CHUNKADAPTERDCAM_H
