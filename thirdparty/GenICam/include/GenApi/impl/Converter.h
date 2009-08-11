//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Hartmut Nebelung
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/Converter.h,v 1.10 2007/08/16 13:56:23 hartmut_nebelung Exp $
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
\brief    Definition of the CConverter class
*/

#ifndef GENAPI_CONVERTER_H
#define GENAPI_CONVERTER_H
#include <Base/GCString.h>
#include "../IFloat.h"
#include "Node.h"
#include "BaseT.h"
#include "ValueT.h"
#include "NodeT.h"
#include "SwissKnife.h"
#include "FloatT.h"


#pragma warning ( push )
#pragma warning ( disable : 4275 ) // non dll-interface XXX used as base for  dll-interface class YYY

namespace GenApi
{
    //*************************************************************
    // CConverter class
    //*************************************************************

    //! IFloat implementation with integrated conversion 
    /*! Works like a Float, but has integrated conversion formulas 
        by which the values are converted before writing 
        and after reading.

    The Representation may be defined using one of the values in ERepresentation.
    The default is _UndefindedRepresentation.
    */
    class GENAPI_DECL CConverterImpl : public  IFloat, public CNodeImpl
    {
    public:
        //! Constructor
        CConverterImpl();
    protected:

        //-------------------------------------------------------------
        // IFloat implementation
        //-------------------------------------------------------------

        // Set feature value
        virtual void InternalSetValue(double Value, bool Verify = true);

        // Get feature value
        virtual double InternalGetValue(bool Verify = false);

        // Get minimum value allowed
        virtual double InternalGetMin();

        // Get maximum value allowed
        virtual double InternalGetMax();

        // Get recommended representation
        virtual  ERepresentation InternalGetRepresentation() const
        {
            if (m_Representation == _UndefindedRepresentation)
                if (m_pIntValue) 
                    m_Representation = m_pIntValue->GetRepresentation();
                else 
                    m_Representation = m_pFloatValue->GetRepresentation();
            return m_Representation;
        }

        // Get the physical unit name
        virtual GenICam::gcstring InternalGetUnit() const;

        // Get the access mode of the node
        virtual EAccessMode InternalGetAccessMode() const;

    public:
        //! Checks if the node is temrina
        virtual bool IsTerminalNode() const 
        { 
            return m_pValue == NULL; 
        }

        //! finalises construction
        virtual void FinalConstruct();

        //-------------------------------------------------------------
        // Initializing
        //-------------------------------------------------------------
    public:
        BEGIN_CONVERT_MAP
            // forwarding the entries to the right swiss knife
            else if (Direction == In && PropertyName == "FormulaTo") 
                return m_pConvertTo->SetProperty("Formula", ValueStr, AttributeStr);
            else if (Direction == In && PropertyName == "FormulaFrom")   
                return m_pConvertFrom->SetProperty("Formula", ValueStr, AttributeStr);
            
            // forwarding the entry to both swissknife
            else if (Direction == In && PropertyName == "pVariable") 
            { 
                m_pConvertFrom->SetProperty(PropertyName, ValueStr, AttributeStr); 
                m_pConvertTo->SetProperty(PropertyName, ValueStr, AttributeStr); 
                return true; 
            } 
            else if (Direction == Out && PropertyName == "pVariable") 
            { 
                m_pConvertFrom->GetProperty(PropertyName, ValueStr, AttributeStr); 
                m_pConvertTo->GetProperty(PropertyName, ValueStr, AttributeStr); 
                return true;
            }
            ADD_VARIABLE_ENTRY(pVariable, (&m_VariableDelegate))
            CONVERT_NODE_REFERENCE(pValue, m_pValue, IValue)
            CONVERT_STRING_ENTRY(Unit, m_Unit)
            CONVERT_ENUM_ENTRY(Representation, m_Representation, ERepresentationClass)
            CONVERT_ENUM_ENTRY(Slope, m_Slope, ESlopeClass)
            CHAIN_CONVERT_MAP(CNodeImpl)
        END_CONVERT_MAP


        // Registers the node
        virtual void Register(GenApi::INodeMapPrivate* const pNodeMap, const char *pNodeType, const char *pName, const char *pNameSpace);

        // determine if the conversion function is increasing or decreasing
        void CheckIncreasing();


    private:
        //! Checks if <Min> has ben set
        bool IsMinUninititialized() const;

        //! Checks if <Max> has ben set
        bool IsMaxUninitialized() const; 

        //-------------------------------------------------------------
        // Member variables
        //-------------------------------------------------------------
        
        //! The SwissKnife formula for Set
        GenICam::gcstring m_FormulaTo;

        //! The SwissKnife formula for Get
        GenICam::gcstring m_FormulaFrom;

        //! The Name of the external Variable
        GenICam::gcstring m_InputName;

        //! The Swiss Knife for Set
        CSwissKnife *m_pConvertTo;

        //! The Swiss Knife for Get
        CSwissKnife *m_pConvertFrom;

        //! Object holding additional Variables
        CVariableDelegate m_VariableDelegate;

        //! The math parser doing all the work
        CMathParser m_MathParser;

        //! Pointer to node providing the value
        IValue *m_pValue;

        //! m_pValue cated to IInteger
        IInteger *m_pIntValue;

        //! m_pValue cated to IFloat
        IFloat *m_pFloatValue; 

        //! recommended representation of the value
        mutable ERepresentation m_Representation;

        //! the physical unit name
        GenICam::gcstring m_Unit;

        //! indicates if the formula is strictly monotonous increasing or decreating
        ESlope m_Slope;
    };

    //! class implementingthe converter object
    class GENAPI_DECL CConverter 
        : public BaseT< ValueT< NodeT < FloatT < CConverterImpl > > > >
    {
    };
}

#pragma warning ( pop )

#endif // GENAPI_CONVERTER_H
