//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/Integer.h,v 1.11 2007/08/16 13:56:24 hartmut_nebelung Exp $
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
\brief    Definition of CInteger and CIntegerRef
*/

#ifndef GENAPI_INTEGER_H
#define GENAPI_INTEGER_H

#include "../IInteger.h"
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
    // CIntegerImpl class
    //*************************************************************

    //! IInteger implementation
    /*! Combines three integer features Value, Min, and Max
    to a scalar feature which can be mapped, e.g. to a slider
    */
    class  GENAPI_DECL CIntegerImpl
        : public IInteger
        , public CNodeImpl
    {
    public:
        //! Constructor
        CIntegerImpl();


        //-------------------------------------------------------------
        // INodePrivate implementation (partially)
        //-------------------------------------------------------------
        virtual bool IsTerminalNode() const { return m_pValue == NULL; }

    protected:
        //-------------------------------------------------------------
        // IBase implementation
        //-------------------------------------------------------------

        virtual EAccessMode InternalGetAccessMode() const;

        //-------------------------------------------------------------
        // IInteger implementation
        //-------------------------------------------------------------

        //! Set feature value
        virtual void InternalSetValue(int64_t Value, bool Verify = true);

        //! Get feature value
        virtual int64_t InternalGetValue(bool Verify = false);

        //! Get minimum value allowed
        virtual int64_t InternalGetMin();

        //! Get maximum value allowed
        virtual int64_t InternalGetMax();

        //! Get increment
        virtual int64_t InternalGetInc();

        //! Get recommended representation
        virtual  ERepresentation InternalGetRepresentation();


        //-------------------------------------------------------------
        // Initializing
        //-------------------------------------------------------------

    public:
        BEGIN_CONVERT_MAP
            CONVERT_NODE_REFERENCE(pValue, m_pValue, IInteger)
            CONVERT_NODE_REFERENCE(pMin, m_pMin, IInteger)
            CONVERT_NODE_REFERENCE(pMax, m_pMax, IInteger)
            CONVERT_NODE_REFERENCE(pInc, m_pInc, IInteger)
            CONVERT_ENTRY(Value, m_Value)
            CONVERT_ENTRY(Min, m_Min)
            CONVERT_ENTRY(Max, m_Max)
            CONVERT_ENTRY(Inc, m_Inc)
            CONVERT_ENUM_ENTRY(Representation, m_Representation, ERepresentationClass)
            CHAIN_CONVERT_MAP(CNodeImpl)
        END_CONVERT_MAP

        //! Initializes the object
        virtual void FinalConstruct();

        

    private:

        //! returns true if no <Min> element was present
        bool IsMinConfigured() 
        { 
            return m_Min != INT64_MAX; 
        }

        //! returns true if no <Max> element was present
        bool IsMaxConfigured() 
        { 
            return m_Max != INT64_MIN; 
        }
        
        //-------------------------------------------------------------
        // Member variables
        //-------------------------------------------------------------

        //! Pointer to node providing the value
        IInteger *m_pValue;

        //! The value if m_pValue is not given
        int64_t m_Value;
        
        //! Pointer to node providing the minimum value
        IInteger *m_pMin;

        //! The minimum if m_pMin is not given
        int64_t m_Min;
        
        //! Pointer to node providing the maximum value
        IInteger *m_pMax;
        
        //! The maximum if m_pMax is not given
        int64_t m_Max;

        //! Pointer to node providing the increment value
        IInteger *m_pInc;
        
        //! The increment if m_pInc is not given
        int64_t m_Inc;

        //! recommended representation of the value
        ERepresentation m_Representation;
    };

    //! implements the Integer node
    class GENAPI_DECL CInteger : public BaseT< ValueT< IntegerT< NodeT < CIntegerImpl> > > >
    {
    };

}

#pragma warning ( pop )

#endif // GENAPI_INTEGER_H
