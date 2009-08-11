//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/SwissKnife.h,v 1.14 2007/12/19 15:00:40 hartmut_nebelung Exp $
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
\brief    Definition  of CSwissKnife
*/

#ifndef GENAPI_SWISSKNIFE_H
#define GENAPI_SWISSKNIFE_H

#include "../IFloat.h"
#include "Node.h"
#include "VariableDelegate.h"
#include "MathParser/MathParser.h"
#include "BaseT.h"
#include "ValueT.h"
#include "NodeT.h"
#include "FloatT.h"

#pragma warning ( push )
#pragma warning ( disable : 4275 ) // non dll-interface XXX used as base for  dll-interface class YYY

namespace GenApi
{
    //*************************************************************
    // CSwissKnife class
    //*************************************************************

    /**
    * \ingroup internal_impl
    *
    * \brief  Specialized SwissKnife for float nodes
    * 
    * Used for formula evaluation with ToPhysical
    * and FromPhysical childs  
    */
    class GENAPI_DECL CSwissKnifeImpl : public IFloat,  public CNodeImpl
    {
    public:
        //! Constructor
        CSwissKnifeImpl();

        //! Destructor
        virtual ~CSwissKnifeImpl();

    protected:
        //-------------------------------------------------------------
        // IBase implementation
        //-------------------------------------------------------------
        
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
                return m_AccessModeCache= NA;
            else
                return m_AccessModeCache = RO;
        }

    public:
        //-------------------------------------------------------------
        // INodePrivate implementation
        //-------------------------------------------------------------

        //! finalizes the construction
        virtual void FinalConstruct();

    protected:
        //-------------------------------------------------------------
        // IFloat implementation
        //-------------------------------------------------------------

        // Set feature value
        #pragma BullseyeCoverage off
        virtual void InternalSetValue(double /*Value*/, bool /*Verify*/ )
        {
            throw LOGICAL_ERROR_EXCEPTION("SwissKnife : %s SetValue failed. SwissKnife is read only", m_Name.c_str() );   
        }
        #pragma BullseyeCoverage on

        // Get feature value
        virtual double InternalGetValue(bool Verify = false);

        // Get minimum value allowed
        virtual double InternalGetMin()
        {
            return m_Min;
        }

        // Get maximum value allowed
        virtual double InternalGetMax()
        {
            return m_Max;
        }

        // Get recommended representation
        virtual  ERepresentation InternalGetRepresentation()
        {
            return m_Representation;
        }

        // Get unit
        virtual GenICam::gcstring InternalGetUnit() const
        {
            return m_Unit;
        }

    public:
        //! Get feature value usint m_InputName as hardcoded variable name
        /*! This is a helper for the implementation of the converter */
        virtual double _GetValue(double input);

        //! Propagate node dependency
        virtual bool PropagateDependency( NodePrivateList_t& Dependents  );

        //-------------------------------------------------------------
        // Initializing
        //-------------------------------------------------------------

        //! Convert map
        BEGIN_CONVERT_MAP
            CONVERT_STRING_ENTRY(Formula, m_Formula)
            CONVERT_STRING_ENTRY(Input, m_InputName)
            ADD_VARIABLE_ENTRY(pVariable, (&m_VariableDelegate))
            CONVERT_ENUM_ENTRY(Representation, m_Representation, ERepresentationClass)
            CONVERT_STRING_ENTRY(Unit, m_Unit)
            CHAIN_CONVERT_MAP(CNodeImpl)
        END_CONVERT_MAP

    protected:
        //-------------------------------------------------------------
        // Member variables
        //-------------------------------------------------------------

        //! minimum value to be stored in the Register
        double  m_Min;

        //! maximum value to be stored in the Register
        double  m_Max;

        //! the Swiss Knife formula
        GenICam::gcstring m_Formula;

        //! A hardcoded variable name
        /*! This is a helper for the implementation of the converter */
        GenICam::gcstring m_InputName;

        //! Object holding the input values
        CVariableDelegate m_VariableDelegate;

        //! The math parser doing all the work
        CMathParser m_MathParser;

        //! the node's representation
        ERepresentation m_Representation;

        //! the physical unit name
        GenICam::gcstring m_Unit;


    };

    class CSwissKnife : public BaseT< ValueT< NodeT< FloatT<  CSwissKnifeImpl > > > >
    {
    };

}

#pragma warning ( pop )

#endif // GENAPI_SWISSKNIFE_H
