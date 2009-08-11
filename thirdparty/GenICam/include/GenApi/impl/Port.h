//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/Port.h,v 1.9 2007/08/16 13:56:26 hartmut_nebelung Exp $
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
\brief    Definition of CPort
*/

#ifndef GENAPI_PORT_H
#define GENAPI_PORT_H

#include "../Types.h"
#include "../IPortConstruct.h"
#include "../IChunkPort.h"
#include "../IString.h"
#include "Node.h"
#include "NodeT.h"
#include "BaseT.h"

namespace GenApi
{

    class CValueCache;

    //*************************************************************
    // CPort class
    //*************************************************************

    //! Standard IPort implementation
    /*! Proxy forwarding all calls to the real implementation
    */
    class GENAPI_DECL CPort_ : public IPortConstruct, public IChunkPort, public CNodeImpl
    {
    public:
        //! Default constructor
        //constructor
        CPort_();

        //! Destructor
        ~CPort_();

        //-------------------------------------------------------------
        // IBase implementation
        //-------------------------------------------------------------

    protected:
        //! Get the access mode of the node
        virtual EAccessMode InternalGetAccessMode() const;

        //-------------------------------------------------------------
        // IPort implementation
        //-------------------------------------------------------------

    public:
        //! Reads a chunk of bytes from the port
        virtual void Read(void *pBuffer, int64_t Address, int64_t Length);

        //! Writes a chunk of bytes to the port
        virtual void Write(const void *pBuffer, int64_t Address, int64_t Length);

        //-------------------------------------------------------------
        // IPortConstruct implementation
        //-------------------------------------------------------------

        //! Sets pointer the real port implementation; this function may called only once
        void SetPortImpl(IPort* pPort);

        //-------------------------------------------------------------
        // IChunkPort implementation
        //-------------------------------------------------------------

        //! Get the Id of the chunk the port should be attached to
        virtual GenICam::gcstring GetChunkID() const;

        //! Determines if the port adapter must perform an endianess swap
        virtual EYesNo GetSwapEndianess();

        //-------------------------------------------------------------
        // Initializing
        //-------------------------------------------------------------
        BEGIN_CONVERT_MAP
            CONVERT_STRING_ENTRY(ChunkID, m_ChunkID)
            CONVERT_NODE_REFERENCE(pChunkID, m_pChunkID, IString)
            CONVERT_ENUM_ENTRY(SwapEndianess, m_SwapEndianess, EYesNoClass)
            CHAIN_CONVERT_MAP(CNodeImpl)
        END_CONVERT_MAP

        //! Returns the value cache
        virtual CValueCache* GetValueCache() const;


    protected:
        //-------------------------------------------------------------
        // Member variables
        //-------------------------------------------------------------

        //! Pointer to node giving access to the real port implementation
        IPort *m_pPort;

        //! The ChunkID 
        GenICam::gcstring m_ChunkID;

        //! Pointer to a Node, which indicates if the node is available
        IString *m_pChunkID;

        //! Pointer the value cache
        mutable GenApi::CValueCache *m_pValueCache;

        //! Determines if the port adapter must perform an endianess swap
        EYesNo m_SwapEndianess;
    };

    class GENAPI_DECL CPort : public BaseT<  NodeT< CPort_ > >
    {
    };

}

#endif // ifndef GENAPI_PORT_H
