//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/FltReg.h,v 1.10.2.1 2008/10/17 11:37:09 hartmut_nebelung Exp $
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
\brief    Definition of CFltReg
*/

#ifndef GENAPI_FLTREG_H
#define GENAPI_FLTREG_H

#include "../IFloat.h"
#include "FloatT.h"
#include "Register.h"

#pragma warning ( push )
#pragma warning ( disable : 4275 ) // non dll-interface XXX used as base for  dll-interface class YYY

namespace GenApi
{
    //*************************************************************
    // CFltReg class
    //*************************************************************

    //! IFloat implementation for a register
    class GENAPI_DECL CFltRegImpl : public IFloat, public CRegisterImpl
    {
    public:
        //! Constructor
        CFltRegImpl();

    protected:
        //-------------------------------------------------------------
        // IFloat implementation
        //-------------------------------------------------------------

        //! Set node value
        virtual void InternalSetValue(double dblValue, bool Verify = true);
        
        //! Get node value
        virtual double InternalGetValue(bool Verify = false);
        
        //! Get minimum value allowed
        virtual double InternalGetMin() const;
        
        //! Get maximum value allowed
        virtual double InternalGetMax() const;
        
        //! Get recommended representation
        virtual  ERepresentation InternalGetRepresentation() const
        {
            return m_Representation;
        }

        //! Get the unit
        virtual GenICam::gcstring InternalGetUnit() const 
        {
            return m_Unit;
        }

        //-------------------------------------------------------------
        // Initializing
        //-------------------------------------------------------------
    public:

        //! Initializes the object
        virtual void FinalConstruct();

        BEGIN_CONVERT_MAP
            CONVERT_ENUM_ENTRY(Endianess, m_Endianess, EEndianessClass)
            CONVERT_STRING_ENTRY(Unit, m_Unit)
            CONVERT_ENUM_ENTRY(Representation, m_Representation, ERepresentationClass)
            CHAIN_CONVERT_MAP(CRegisterImpl)
        END_CONVERT_MAP

    protected:
        //! helper to handle the register access incl. swapping
        void ReadReg(uint8_t* pBuffer, uint8_t* ValueBytes);

        //! helper to handle the register access incl. swapping
        void WriteReg(uint8_t* pBuffer, uint8_t* ValueBytes);    

        //-------------------------------------------------------------
        // Memeber variables
        //-------------------------------------------------------------

        //! minimum value to be stored in the Register
        mutable double m_Min;

        //! maximum value to be stored in the Register
        mutable double m_Max;

        //! The float's reprsentation
        mutable ERepresentation  m_Representation;

        //! endianess of the register
        mutable EEndianess m_Endianess;

        //! the physical unit name
        GenICam::gcstring m_Unit;
    };

    //! the clkass implementing the FltReg node
    class GENAPI_DECL CFltReg 
        : public BaseT< ValueT< FloatT <  RegisterT < NodeT< CFltRegImpl > > > > >
    {
    };
}

#pragma warning ( pop )

#endif // GENAPI_FLTREG_H
