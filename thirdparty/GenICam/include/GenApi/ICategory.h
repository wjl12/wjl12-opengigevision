//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/ICategory.h,v 1.13 2007/08/16 13:56:39 hartmut_nebelung Exp $
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
\brief  Definition of interface ICategory and types FeatureList_t :
\ingroup GenApi_PublicInterface
*/


#ifndef GENAPI_ICATEGORY_H
#define GENAPI_ICATEGORY_H

#include "GenApiDll.h"
#include "Types.h"
#include "IValue.h"
#include <Base/GCString.h>
#include "Container.h"

#include <assert.h>

#pragma warning ( push )
#pragma warning ( disable : 4251 ) // XXX needs to have dll-interface to be used by clients of class YYY


namespace GenApi
{

    interface INode;


    //*************************************************************
    // ICategory interface
    //*************************************************************

    /**
    \brief Gives access to a category node
    \ingroup GenApi_PublicInterface
    */
    interface   ICategory  : virtual public IValue
    {

        //! Get all features of the category (including sub-categories) 
        virtual void GetFeatures(FeatureList_t &Features) const =  0;

    };


    //*************************************************************
    // CCategoryRef class
    //*************************************************************

    /**
    \internal
    \brief Reference to an ICategory pointer
    \ingroup GenApi_PublicImpl
    */
    template <class T>
    class CCategoryRefT : public CValueRefT<T>
    {
        typedef CValueRefT<T> ref;

    public:
        /*--------------------------------------------------------*/
        // ICategory
        /*--------------------------------------------------------*/

        //! Get all features of the category (including sub-categories) 
        virtual void GetFeatures(FeatureList_t &Features) const
        {
            if(ref::m_Ptr)
                return ref::m_Ptr->GetFeatures(Features);
            else
                throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
        }

    };

    //! Reference to an ICategory pointer
    //! \ingroup GenApi_PublicImpl
    typedef CCategoryRefT<ICategory> CCategoryRef;  

}
#pragma warning ( pop )

#endif // ifndef GENAPI_ICATEGORY_H
