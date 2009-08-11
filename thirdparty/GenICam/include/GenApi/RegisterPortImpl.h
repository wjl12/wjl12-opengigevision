//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/RegisterPortImpl.h,v 1.4 2007/08/16 13:56:42 hartmut_nebelung Exp $
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
\brief    Definition of CRegisterPortImpl
\ingroup GenApi_PublicUtilities
*/

#ifndef GENAPI_REGISTERPORTIMPL_H
#define GENAPI_REGISTERPORTIMPL_H

#include "PortImpl.h"
#include "Log.h"
#include "impl/Compatibility.h"

namespace GenApi
{
    //*************************************************************
    // CRegisterPortImpl class
    //*************************************************************

    /**
    \brief Standard implementation for a port using a register based transport layer
    //! \ingroup GenApi_PublicUtilities
    */
    class GENAPI_DECL CRegisterPortImpl : public CPortImpl 
    {
    public:
        //! Constructor
        CRegisterPortImpl(int MaxNumQuadlets = 1, bool TransportLayerSwapsEndianess = false);

        //! Destructor
        virtual ~CRegisterPortImpl();

        /*---------------------------------------------------------------*/
        // IBase ==> You need to override this method
        /*---------------------------------------------------------------*/

        //! Get the access mode of the node
        /*! Driver closed => NI, Driver open => RW, analysing a struct, RO */
        virtual EAccessMode GetAccessMode() const = 0;

        /*---------------------------------------------------------------*/
        // IRegisterPort ==> You need to override these methods
        /*---------------------------------------------------------------*/

        //! Reads a array of quadlets from the port
        virtual void ReadRegister(uint32_t *pRegisters, int64_t Address, int64_t Length) = 0;

        //! Writes an array of quadlets to the port
        virtual void WriteRegister(const uint32_t *pRegisters, int64_t Address, int64_t Length) = 0;

        /*---------------------------------------------------------------*/
        // IPort
        /*---------------------------------------------------------------*/

        //! Reads a chunk of bytes from the port
        virtual void Read(void *pBuffer, int64_t Address, int64_t Length);

        //! Writes a chunk of bytes to the port
        virtual void Write(const void *pBuffer, int64_t Address, int64_t Length);

        /*---------------------------------------------------------------*/
        // IPortConstruct implementation (without IPort & IBase)
        /*---------------------------------------------------------------*/

        //! Sets pointer the real port implementation; this function may called only once
        virtual void SetPortImpl(IPort* pPort);

        // Invalidate the node
        void InvalidateNode();
    
    protected:
        //! Reads a array of quadlets from the port
        virtual void InternalReadRegister(uint32_t *pRegisters, int64_t Address, int64_t Length);

        //! Writes an array of quadlets to the port
        virtual void InternalWriteRegister(const uint32_t *pRegisters, int64_t Address, int64_t Length);

        //! Pointer to the node holding a reference to this implementation
        CNodePtr m_ptrPort;

        //! The maximum nuber of quadlets ReadRegister can take
        int m_MaxNumQuadlets;
        
        //! Inidicates if the transport layer driver flips the quadlets coming from thte network
        /*! False means that the quadlets are delivered as they are coming via the network
            True means that the transport layer driver does a little/big endian swap before delivering the quadlets 
        */
        bool m_TransportLayerSwapsEndianess;

        //! Buffer holding the quadlets from the network
        uint32_t *m_pQuadletBuffer; 

        //! Buffer holding the quadlets from the network
        uint32_t *m_pQuadletSwapBuffer; 

        //! Indicates if an edianess swap must be performed
        bool m_EndianessSwap;

#ifdef LOG4C
        //! Logger for messages concerning the port access
        log4cpp::Category *m_pPortLog;
#endif

    };
}


#endif // ifndef GENAPI_PORTIMPL_H
