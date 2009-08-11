//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/IString.h,v 1.14 2007/08/16 13:56:41 hartmut_nebelung Exp $
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
\brief    Definition of interface IString
\ingroup GenApi_PublicInterface
*/

#ifndef GENAPI_ISTRING_H
#define GENAPI_ISTRING_H

#include <Base/GCString.h>
#include "GenApiDll.h"
#include "Types.h"
#include "IValue.h"

#pragma warning ( push )
#pragma warning ( disable : 4251 ) // XXX needs to have dll-interface to be used by clients of class YYY

namespace GenApi
{
    //*************************************************************
    // IString interface
    //*************************************************************

    /**
    \brief Interface for string properties
    \ingroup GenApi_PublicInterface
    */
    interface   IString  : virtual public IValue
    {
        //! Set node value
        virtual void SetValue(const GenICam::gcstring& Value, bool Verify = true) = 0;

        //! Set node value
        virtual IString& operator=(const GenICam::gcstring& Value) = 0;

        //! Get node value
        virtual GenICam::gcstring GetValue(bool Verify = false) = 0;

        //! Get node value
        virtual GenICam::gcstring operator()() = 0;

        //! Get node value
        virtual GenICam::gcstring operator*() = 0;

    };
    //*************************************************************
    // CStringRef class
    //*************************************************************

    /**
    \internal
    \brief Reference to an IEnumEntry pointer
    \ingroup GenApi_PublicImpl
    */
    template <class T>
    class CStringRefT : public CValueRefT<T>
    {
        typedef CValueRefT<T> ref;

    public:
        /*--------------------------------------------------------*/
        // IString
        /*--------------------------------------------------------*/

        //! Set node value
        virtual void SetValue(const GenICam::gcstring& Value, bool Verify = true)
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->SetValue(Value, Verify);
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Set node value
        virtual IString& operator=(const GenICam::gcstring& Value)
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->operator=(Value);
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Get node value
        virtual GenICam::gcstring GetValue(bool Verify = false)
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->GetValue(Verify);
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Get node value
        virtual GenICam::gcstring operator()()
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->operator()();
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Get node value
        virtual GenICam::gcstring operator*()
        {
            if(ref::m_Ptr)
                return ref::m_Ptr-> operator*();
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

    };

    //! Reference to an IString pointer
    //! \ingroup GenApi_PublicImpl
    typedef CStringRefT<IString> CStringRef;  


}

#pragma warning ( pop )

#endif // ifndef GENAPI_ISTRING_H
