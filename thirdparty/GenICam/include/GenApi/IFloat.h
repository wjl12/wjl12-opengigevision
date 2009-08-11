//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/IFloat.h,v 1.16 2007/08/16 13:56:40 hartmut_nebelung Exp $
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
\brief    Definition of the IFloat interface
\ingroup GenApi_PublicInterface
*/

#ifndef GENAPI_IFLOAT_H
#define GENAPI_IFLOAT_H

#include "GenApiDll.h"
#include "Types.h"
#include "IValue.h"

#pragma warning ( push )
#pragma warning ( disable : 4251 ) // XXX needs to have dll-interface to be used by clients of class YYY

namespace GenApi
{
    //*************************************************************
    // IFloat interface
    //*************************************************************

    /**
    \brief Interface for float properties
    \ingroup GenApi_PublicInterface
    */
    interface   IFloat  : virtual public IValue
    {
        //! Set node value
        virtual void SetValue(double Value, bool Verify = true) = 0;

        //! Set node value
        virtual IFloat& operator=(double Value) = 0;

        //! Get node value
        virtual double GetValue(bool Verify = false) = 0;

        //! Get node value
        virtual double operator()() = 0;

        //! Get node value
        virtual double operator*() = 0;

        //! Get minimum value allowed
        virtual double GetMin() = 0;

        //! Get maximum value allowed
        virtual double GetMax() = 0;

        //! Get recommended representation
        virtual  ERepresentation GetRepresentation() = 0;

        //! Get the physical unit name
        virtual GenICam::gcstring GetUnit() const = 0;
    };


    //*************************************************************
    // CFloatRef class
    //*************************************************************

    /**
    \internal
    \brief Reference to an IEnumEntry pointer
    \ingroup GenApi_PublicImpl
    */
    template <class T>
    class CFloatRefT : public CValueRefT<T>
    {
        typedef CValueRefT<T> ref;

    public:
        /*--------------------------------------------------------*/
        // IFloat
        /*--------------------------------------------------------*/

        //! Set node value
        virtual void SetValue(double Value, bool Verify = true)
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->SetValue(Value, Verify);
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Set node value
        virtual IFloat& operator=(double Value)
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->operator=(Value);
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Get node value
        virtual double GetValue(bool Verify = false)
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->GetValue(Verify);
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Get node value
        virtual double operator()()
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->operator()();
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Get node value
        virtual double operator*()
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->operator*();
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Get minimum value allowed
        virtual double GetMin()
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->GetMin();
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Get maximum value allowed
        virtual double GetMax()
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->GetMax();
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

        //! Get unit
        virtual GenICam::gcstring GetUnit() const
        {
            if (ref::m_Ptr)
                return ref::m_Ptr->GetUnit();
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

    };

    //! Reference to an IFloat pointer
    //! \ingroup GenApi_PublicImpl
    typedef CFloatRefT<IFloat> CFloatRef;  


}

#pragma warning ( pop )

#endif // ifndef GENAPI_IFLOAT_H
