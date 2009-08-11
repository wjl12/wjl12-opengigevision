//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/Pointer.h,v 1.19.2.1 2009/02/02 13:24:45 hopfner Exp $
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
\brief    Definition of template CPointer
\ingroup GenApi_PublicUtilities
*/

#ifndef GENAPI_POINTER_H
#define GENAPI_POINTER_H


#include <Base/GCBase.h>

#include "IBase.h"
#include "INode.h"
#include "IValue.h"
#include "ICategory.h"
#include "IInteger.h"
#include "IFloat.h"
#include "IRegister.h"
#include "IEnumeration.h"
#include "IEnumEntry.h"
#include "IBoolean.h"
#include "IPort.h"
#include "IPortConstruct.h"
#include "IChunkPort.h"
#include "IString.h"
#include "INodeMap.h"
#include "INodeMapDyn.h"
#include "IDestroy.h"
#include "IDeviceInfo.h"
#include "ISelector.h"
#include "ICommand.h"

#include <assert.h>

namespace GenApi
{
    //*************************************************************
    // CPointer class
    //*************************************************************

    /**
    \brief Encapsulates a GenApi pointer dealing with the dynamic_cast automatically
    \ingroup GenApi_PublicUtilities
    */
    template <class T, class B = IBase>
    class CPointer
    {

    public:
        //! Default constructor.
        CPointer(void) throw()
            : m_pT( NULL )
        {
        }

        //! Constructor from INode pointer type.
        CPointer( B *pB ) 
            : m_pT( dynamic_cast<T*>(pB) )
        {
        }

        virtual ~CPointer(void) 
        {
        }

        //! Assign INode Pointer
        void operator=( B *pB ) 
        {
            m_pT = dynamic_cast<T*>(pB);
        }

        //! Dereferencing
        operator T*(void) const
        {
            if (NULL == m_pT)
                throw LOGICAL_ERROR_EXCEPTION( "NULL pointer dereferenced" );
            return m_pT;
        }

        //! Dereferencing
        T& operator*(void) const
        {
            if (NULL == m_pT)
                throw LOGICAL_ERROR_EXCEPTION( "NULL pointer dereferenced" );
            return *m_pT;
        }

        //! Dereferencing
        T& operator()(void) const
        {
            if (NULL == m_pT)
                throw LOGICAL_ERROR_EXCEPTION( "NULL pointer dereferenced" );
            return *m_pT;
        }

        //! Dereferencing
        T* operator->(void) const
        {
            if (NULL == m_pT)
                throw LOGICAL_ERROR_EXCEPTION( "NULL pointer dereferenced" );
            return m_pT;
        }

        //! true if the pointer is valid
        bool IsValid() const throw()
        {
            return m_pT != NULL;
        }

        //! true if the pointer is valid
        operator bool(void) const throw()
        {
            return m_pT != NULL;
        }

        //! pointer equal
        bool operator==(T* pT) const throw()
        {
            return m_pT == pT;
        }

        //! pointer equal
        bool operator==(int nMustBeNull) const
        {
            if (0 != nMustBeNull)
                throw LOGICAL_ERROR_EXCEPTION( "argument must be NULL" );
            return NULL == m_pT;
        }


    protected:

        //! Underlying raw pointer.
        T* m_pT;
    };

    //*************************************************************
    // Smartpointer for all interface
    //*************************************************************
    
    //! \addtogroup GenApi_PublicUtilities
    //! \{

    //! SmartPointer for IBase interface pointer
    typedef CPointer<IBase> CBasePtr;

    //! SmartPointer for INode interface pointer
    typedef CPointer<INode> CNodePtr;

    //! SmartPointer for IValue interface pointer
    typedef CPointer<IValue> CValuePtr;

    //! SmartPointer for ICategory interface pointer
    typedef CPointer<ICategory> CCategoryPtr;

    //! SmartPointer for IInteger interface pointer
    typedef CPointer<IInteger> CIntegerPtr;

    //! SmartPointer for IBoolean interface pointer
    typedef CPointer<IBoolean> CBooleanPtr;

    //! SmartPointer for IFloat interface pointer
    typedef CPointer<IFloat> CFloatPtr;

    //! SmartPointer for IString interface pointer
    typedef CPointer<IString> CStringPtr;

    //! SmartPointer for IRegister interface pointer
    typedef CPointer<IRegister> CRegisterPtr;

    //! SmartPointer for IEnumeration interface pointer
    typedef CPointer<IEnumeration> CEnumerationPtr;

    //! SmartPointer for IEnumEntry interface pointer
    typedef CPointer<IEnumEntry> CEnumEntryPtr;

    //! SmartPointer for IPort interface pointer
    typedef CPointer<IPort> CPortPtr;

    //! SmartPointer for IChunkPort interface pointer
    typedef CPointer<IChunkPort> CChunkPortPtr;

    //! SmartPointer for INodeMap interface pointer
    typedef CPointer<INodeMap, INodeMap> CNodeMapPtr;

    //! SmartPointer for INodeMapDyn interface pointer
    typedef CPointer<INodeMapDyn, INodeMap> CNodeMapDynPtr;

    //! SmartPointer for IDeviceInfo interface pointer
    typedef CPointer<IDeviceInfo, INodeMap> CDeviceInfoPtr;

    //! SmartPointer for ISelector interface pointer
    typedef CPointer<ISelector> CSelectorPtr;

    //! SmartPointer for ICommand interface pointer
     typedef CPointer<ICommand> CCommandPtr;

    //! \}

    //! \addtogroup GenApi_PublicImpl
    //! \{

    //! SmartPointer for IPortConstruct interface pointer
    typedef CPointer<IPortConstruct> CPortConstructPtr;


    //! Returns the name of the main interface as string
    inline GenICam::gcstring GetInterfaceName(IBase *pBase)
    {
#pragma warning( push )
#pragma warning ( disable : 4068 ) // unknown pragma; refers to BullsEyeCoverage

       if( dynamic_cast<GenApi::IInteger*>(pBase) )
            return GenICam::gcstring("IInteger");
        else if( dynamic_cast<GenApi::IFloat*>(pBase) )
            return GenICam::gcstring("IFloat");
        else if( dynamic_cast<GenApi::IString*>(pBase) )
            return GenICam::gcstring("IString");
        else if( dynamic_cast<GenApi::IEnumeration*>(pBase) )
            return GenICam::gcstring("IEnumeration");
        else if( dynamic_cast<GenApi::IBoolean*>(pBase) )
            return GenICam::gcstring("IBoolean");
        else if( dynamic_cast<GenApi::ICommand*>(pBase) )
            return GenICam::gcstring("ICommand");
        else if( dynamic_cast<GenApi::IRegister*>(pBase) )
            return GenICam::gcstring("IRegister");
        else if( dynamic_cast<GenApi::IEnumEntry*>(pBase) )
            return GenICam::gcstring("IEnumEntry");
        else if( dynamic_cast<GenApi::IPort*>(pBase) )
            return GenICam::gcstring("IPort");
        // these must be the last
        #pragma BullseyeCoverage off
        else if( dynamic_cast<GenApi::INode*>(pBase) )
            return GenICam::gcstring("INode");
        else if( dynamic_cast<GenApi::IValue*>(pBase) )
            return GenICam::gcstring("IValue");
        else if( dynamic_cast<GenApi::IBase*>(pBase) )
            return GenICam::gcstring("IBase");
        else
            return GenICam::gcstring("!!! unknown interface type !!!");
        #pragma BullseyeCoverage on
#pragma warning( pop )
    }

    //! \}


}

#endif // ifndef GENAPI_POINTER_H
