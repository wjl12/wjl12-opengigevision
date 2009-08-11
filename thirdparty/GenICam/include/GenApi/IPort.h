//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/IPort.h,v 1.14 2007/08/16 13:56:41 hartmut_nebelung Exp $
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
\brief    Definition of interface IPort
\ingroup GenApi_PublicInterface
*/

#ifndef GENAPI_IPORT_H
#define GENAPI_IPORT_H

#include "GenApiDll.h"
#include "Types.h"
#include "IBase.h"

#pragma warning ( push )
#pragma warning ( disable : 4251 ) // XXX needs to have dll-interface to be used by clients of class YYY

namespace GenApi
{
    //*************************************************************
    // IPort interface
    //*************************************************************

    /**
    \brief Interface for ports
    \ingroup GenApi_PublicInterface
    */
    interface IPort  : virtual public IBase
    {
        //! Reads a chunk of bytes from the port
        virtual void Read(void *pBuffer, int64_t Address, int64_t Length) = 0;

        //! Writes a chunk of bytes to the port
        virtual void Write(const void *pBuffer, int64_t Address, int64_t Length) = 0;
    };

    //*************************************************************
    // CPortRef class
    //*************************************************************

    /**
    \internal
    \brief Reference to an IPort pointer
    \ingroup GenApi_PublicImpl
    */
    template <class T>
    class CPortRefT : public CBaseRefT<T>
    {
        typedef CBaseRefT<T> ref;

    public:
        /*--------------------------------------------------------*/
        // IPort
        /*--------------------------------------------------------*/

        //! Reads a chunk of bytes from the port
        virtual void Read(void *pBuffer, int64_t Address, int64_t Length)
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->Read(pBuffer, Address, Length);
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Writes a chunk of bytes to the port
        virtual void Write(const void *pBuffer, int64_t Address, int64_t Length)
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->Write(pBuffer, Address, Length);
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

    };

    //! Reference to an IEnumEntry pointer
    //! \ingroup GenApi_PublicImpl
    typedef CPortRefT<IPort> CPortRef;  



}

#pragma warning ( pop )

#endif // ifndef GENAPI_IPORT_H
