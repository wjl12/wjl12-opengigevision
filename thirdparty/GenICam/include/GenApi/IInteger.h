//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/IInteger.h,v 1.14 2007/08/16 13:56:40 hartmut_nebelung Exp $
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
\brief    Definition of the interface IInteger.
\ingroup GenApi_PublicInterface
*/

#ifndef GENAPI_IIINTEGER_H
#define GENAPI_IIINTEGER_H

#include "GenApiDll.h"
#include "Types.h"
#include "IValue.h"

#pragma warning ( push )
#pragma warning ( disable : 4251 ) // XXX needs to have dll-interface to be used by clients of class YYY

namespace GenApi
{
    //*************************************************************
    // IInteger interface
    //*************************************************************

    /**
    \brief Interface for integer properties
    \ingroup GenApi_PublicInterface
    */
    interface   IInteger : virtual public IValue
    {
        //! Set node value
        virtual void SetValue(int64_t Value, bool Verify = true) = 0;

        //! Set node value
        virtual IInteger& operator=(int64_t Value) = 0;

        //! Get node value
        virtual int64_t GetValue(bool Verify = false) = 0;

        //! Get node value
        virtual int64_t operator()() = 0;

        //! Get node value
        virtual int64_t operator*() = 0;

        //! Get minimum value allowed
        virtual int64_t GetMin() = 0;

        //! Get maximum value allowed
        virtual int64_t GetMax() = 0;

        //! Get increment
        virtual int64_t GetInc() = 0;

        //! Get recommended representation
        virtual  ERepresentation GetRepresentation() = 0;
    };

    //*************************************************************
    // CIntegerRef class
    //*************************************************************

    /**
    \internal
    \brief Reference to an IInteger pointer
    \ingroup GenApi_PublicImpl
    */
    template <class T>
    class CIntegerRefT : public CValueRefT<T>
    {
        typedef CValueRefT<T> ref;

    public:
        /*--------------------------------------------------------*/
        // IInteger
        /*--------------------------------------------------------*/

        //! Set node value
        virtual void SetValue(int64_t Value, bool Verify = true)
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->SetValue(Value, Verify);
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Set node value
        virtual IInteger& operator=(int64_t Value)
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->operator=(Value);
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Get node value
        virtual int64_t GetValue(bool Verify = false)
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->GetValue(Verify);
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Get node value
        virtual int64_t operator()()
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->operator()();
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Get node value
        virtual int64_t operator*()
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->operator*();
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Get minimum value allowed
        virtual int64_t GetMin()
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->GetMin();
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Get maximum value allowed
        virtual int64_t GetMax()
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->GetMax();
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Get increment
        virtual int64_t GetInc()
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->GetInc();
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Get recommended representation
        virtual  ERepresentation GetRepresentation()
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->GetRepresentation();
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

    };

    //! Reference to an IInteger pointer
    //! \ingroup GenApi_PublicImpl
    typedef CIntegerRefT<IInteger> CIntegerRef;
}

#pragma warning ( pop )

#endif // ifndef GENAPI_IIINTEGER_H
