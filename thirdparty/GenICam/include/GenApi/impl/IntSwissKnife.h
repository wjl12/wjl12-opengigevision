//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/IntSwissKnife.h,v 1.15 2007/12/19 15:01:33 hartmut_nebelung Exp $
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
\brief    Definition of CIntSwissKnife
*/

#ifndef GENAPI_INTSWISSKNIFE_H
#define GENAPI_INTSWISSKNIFE_H

#include "VariableDelegate.h"
#include "MathParser/Int64MathParser.h"
#include "Node.h"
#include "BaseT.h"
#include "ValueT.h"
#include "IntegerT.h"
#include "NodeT.h"

#pragma warning ( push )
#pragma warning ( disable : 4275 ) // non dll-interface XXX used as base for  dll-interface class YYY

namespace GenApi
{
    //*************************************************************
    // CIntSwissKnife class
    //*************************************************************

    /**
    * \ingroup internal_impl
    *
    * \brief  IInteger implementation for a SwissKnife used for formula evaluation
    */
    class GENAPI_DECL CIntSwissKnifeImpl : public IInteger, public CNodeImpl
    {
    public:
        //! Constructor
        CIntSwissKnifeImpl();

        //! Destructor
        virtual ~CIntSwissKnifeImpl();

        //-------------------------------------------------------------
        // IBase implementation
        //-------------------------------------------------------------

    protected:
        // Get access mode
        virtual EAccessMode InternalGetAccessMode() const
        {
            const EAccessMode mode = CNodeImpl::InternalGetAccessMode();
            const EAccessMode vmode = m_VariableDelegate.GetAccessMode();
            if (mode == NI
                           #pragma BullseyeCoverage off
                           || vmode == NI
                           #pragma BullseyeCoverage on
                                          )
                return m_AccessModeCache = NI;
            if (mode == NA
                           #pragma BullseyeCoverage off
                           || vmode == NA
                           #pragma BullseyeCoverage on
                                          )               
                return m_AccessModeCache = NA;
 
            else
                return m_AccessModeCache = RO;

        }

        //-------------------------------------------------------------
        // INodePrivate implementation
        //-------------------------------------------------------------

    public:
        //! Initializes the object
        virtual void FinalConstruct();

        //Propagate node dependency
        virtual bool PropagateDependency( NodePrivateList_t& Dependents  );
        
        //-------------------------------------------------------------
        // IInteger implementation
        //-------------------------------------------------------------

        //! Get feature value usint m_InputName as hardcoded variable name
        /*! This is a helper for the implementation of the converter */
        virtual int64_t _GetValue(int64_t input, bool Verify = false);


    protected:
        //! Set feature value
        #pragma BullseyeCoverage off
        virtual void InternalSetValue(int64_t /*Value*/, bool /* Verify = true */)
        {
            throw LOGICAL_ERROR_EXCEPTION("IntSwissKnife : %s SetValue failed. IntSwissKnife is read only", m_Name.c_str() );   
        }
        #pragma BullseyeCoverage on

        //! Get feature value
        virtual int64_t InternalGetValue(bool Verify = false);


        //! Get minimum value allowed
        virtual int64_t InternalGetMin() const
        {
            return m_Min;
        }

        //! Get maximum value allowed
        virtual int64_t InternalGetMax() const
        {
            return m_Max;
        }

        //! Get increment
        virtual int64_t InternalGetInc() const
        {
            return 1LL;
        }

        //! Get recommended representation
        virtual  ERepresentation InternalGetRepresentation() const
        {
            return m_Representation;
        }




    public:
        //-------------------------------------------------------------
        // Initializing
        //-------------------------------------------------------------
        //! Convert map
        BEGIN_CONVERT_MAP
            CONVERT_STRING_ENTRY(Formula, m_Formula)
            CONVERT_STRING_ENTRY(Input, m_InputName)
            ADD_VARIABLE_ENTRY(pVariable, (&m_VariableDelegate))
            CONVERT_ENUM_ENTRY(Representation, m_Representation, ERepresentationClass)
            CHAIN_CONVERT_MAP(CNodeImpl)
        END_CONVERT_MAP

    protected:
        // inititialize when childs are initialized
        //-------------------------------------------------------------
        // Memeber variables
        //-------------------------------------------------------------
        //! minimum value to be stored in the Register
        int64_t  m_Min;

        //! maximum value to be stored in the Register
        int64_t  m_Max;

        GenICam::gcstring m_Formula;

        CVariableDelegate m_VariableDelegate;
        CInt64MathParser m_MathParser;
        ERepresentation m_Representation;

        //! A hardcoded variable name
        /*! This is a helper for the implementation of the converter */
        GenICam::gcstring m_InputName;

    };

    class CIntSwissKnife : public BaseT< ValueT< NodeT< IntegerT< CIntSwissKnifeImpl > > > >
    {
    };

};

#pragma warning ( pop )

#endif // GENAPI_INTSWISSKNIFE_H
