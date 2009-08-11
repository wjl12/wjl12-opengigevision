//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/IEnumEntry.h,v 1.14 2007/08/16 13:56:39 hartmut_nebelung Exp $
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
\ingroup GenApi_PublicInterface
*/

#ifndef GENAPI_IENUMENTRY_H
#define GENAPI_IENUMENTRY_H


#include <Base/GCString.h>
#include "GenApiDll.h"
#include "IValue.h"

#pragma warning ( push )
#pragma warning ( disable : 4251 ) // XXX needs to have dll-interface to be used by clients of class YYY

namespace GenApi
{

    //*************************************************************
    // IEnumEntry interface
    //*************************************************************

    /**
    \brief Interface of single enum value
    \ingroup GenApi_PublicInterfa
    */
    /*! Maps of Enum Values to symbolic values */
    interface   IEnumEntry  : virtual public IValue
    {

    public:
        //! Get numeric enum value
        virtual int64_t GetValue() = 0;
        //! Get symbolic enum value
        virtual GenICam::gcstring GetSymbolic()const = 0;
    };

    //*************************************************************
    // CEnumEntryRef class
    //*************************************************************

    /**
    \internal
    \brief Reference to an IEnumEntry pointer
    \ingroup GenApi_PublicImpl
    */
    template <class T>
    class CEnumEntryRefT : public CValueRefT<T>
    {
        typedef CValueRefT<T> ref;

    public:
        /*--------------------------------------------------------*/
        // IEnumEntry
        /*--------------------------------------------------------*/

        //! Get numeric enum value
        virtual int64_t GetValue()
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->GetValue();
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

        //! Get symbolic enum value
        virtual    GenICam::gcstring GetSymbolic()const
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->GetSymbolic();
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

    };

    //! Reference to an IEnumEntry pointer
    //! \ingroup GenApi_PublicImpl
    typedef CEnumEntryRefT<IEnumEntry> CEnumEntryRef;  


}

#pragma warning ( pop )

#endif // ifndef GENAPI_IENUMENTRY_H
