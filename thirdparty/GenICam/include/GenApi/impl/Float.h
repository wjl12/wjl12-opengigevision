//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Hartmut Nebelung
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/Float.h,v 1.11 2007/08/16 13:56:24 hartmut_nebelung Exp $
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
\brief    Definition of the CFloat class
*/

#ifndef GENAPI_FLOAT_H
#define GENAPI_FLOAT_H

#include <Base/GCString.h>
#include "../IFloat.h"
#include "Node.h"
#include "BaseT.h"
#include "ValueT.h"
#include "NodeT.h"
#include "FloatT.h"

#pragma warning ( push )
#pragma warning ( disable : 4275 ) // non dll-interface XXX used as base for  dll-interface class YYY


namespace GenApi
{
    //*************************************************************
    // CFloat class
    //*************************************************************

    //! IFloat implementation
    /*! Combines a three float features Value, Min, and Max
    to a scalar feature which can be mapped, e.g. to a slider.
    The properties may define constant value for Min, Max, Inc and Value.
    Or these can be references to nodes giving the values, then the
    properties pMin, pMax, pInc and pValue must be configured. Any
    combination is allowed. Value defaults to zero, Inc to epsilon 
    (smallest effective increment from 1.0), Min to minimum of the 
    specific float implementation

    The Representation may be defined using one of the values in ERepresentation.
    The default is _UndefindedRepresentation.
    */
    class GENAPI_DECL CFloatImpl : public  IFloat, public CNodeImpl
    {
    public:
        //! Constructor
        CFloatImpl();

    protected:

        //-------------------------------------------------------------
        // IFloat implementation
        //-------------------------------------------------------------

        // Set feature value
        virtual void InternalSetValue(double Value, bool Verify = true);

        // Get feature value
        virtual double InternalGetValue(bool Verify = false) const;

        // Get minimum value allowed
        virtual double InternalGetMin() const;

        // Get maximum value allowed
        virtual double InternalGetMax() const;

        // Get recommended representation
        virtual  ERepresentation InternalGetRepresentation() const
        {
            if (m_Representation == _UndefindedRepresentation)
                if (m_pValue) 
                    m_Representation = m_pValue->GetRepresentation();
            
            return m_Representation;
        }

        // Get the physical unit name
        virtual GenICam::gcstring InternalGetUnit() const;

        // Get the access mode of the node
        virtual EAccessMode InternalGetAccessMode() const;

    public:
        //! checks if the node is termins
        virtual bool IsTerminalNode() const { return m_pValue == NULL; }

        //! finishes the constriction of the node
        virtual void FinalConstruct();

        //-------------------------------------------------------------
        // Initializing
        //-------------------------------------------------------------
    public:
        BEGIN_CONVERT_MAP
            CONVERT_NODE_REFERENCE(pValue, m_pValue, IFloat)
            CONVERT_NODE_REFERENCE(pMin, m_pMin, IFloat)
            CONVERT_NODE_REFERENCE(pMax, m_pMax, IFloat)
            CONVERT_STRING_ENTRY(Unit, m_Unit)
            CONVERT_ENTRY(Value, m_Value)
            CONVERT_ENTRY(Min, m_Min)
            CONVERT_ENTRY(Max, m_Max)
            CONVERT_ENUM_ENTRY(Representation, m_Representation, ERepresentationClass)
            CHAIN_CONVERT_MAP(CNodeImpl)
        END_CONVERT_MAP


    private:
        //! Checks if <Min> was set
        bool IsMinConfigured() const;

        //! Checks if <Max> was set
        bool IsMaxConfigured() const; 

        //-------------------------------------------------------------
        // Member variables
        //-------------------------------------------------------------
        
        //! The value of the node
        mutable double m_Value;

        //! Pointer to node providing the value
        IFloat *m_pValue;
        
        //! Pointer to node providing the minimum value
        IFloat *m_pMin;
        
        //! The minimum value
        mutable double m_Min;

        //! Pointer to node providing the maximum value
        IFloat *m_pMax;
        
        //! The maximum value
        mutable double m_Max;

        //! recommended representation of the value
        mutable ERepresentation m_Representation;
        
        //! the physical unit name
        GenICam::gcstring m_Unit;
    };

    //! class implemetning the Float node
    class GENAPI_DECL CFloat 
        : public BaseT< ValueT< NodeT < FloatT < CFloatImpl > > > >
    {
    };
}

#pragma warning ( pop )

#endif // GENAPI_FLOAT_H
