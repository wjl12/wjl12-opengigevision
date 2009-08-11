//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/IntReg.h,v 1.10 2007/08/16 13:56:25 hartmut_nebelung Exp $
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
\brief    Definition of CIntReg
*/

#ifndef GENAPI_INTREG_H
#define GENAPI_INTREG_H

#include "Register.h"
#include "../IRegister.h"
#include "BaseT.h"
#include "ValueT.h"
#include "NodeT.h"
#include "IntegerT.h"
#include "RegisterT.h"

#pragma warning ( push )
#pragma warning ( disable : 4275 ) // non dll-interface XXX used as base for  dll-interface class YYY

namespace GenApi
{
    //*************************************************************
    // CIntRegImpl class
    //*************************************************************

    /**
    * \ingroup internal_impl
    * \brief IInteger implementation for a register
    *
    * Maps an integer (signed or unsigned) to a register
    */
    class GENAPI_DECL CIntRegImpl
        : public IInteger
        , public CRegisterImpl
    {
    public:
        //! Constructor
        CIntRegImpl();

        //-------------------------------------------------------------
        // INodePrivate implementation
        //-------------------------------------------------------------

        //! Initializes the object
        virtual void FinalConstruct();

    protected:

        //-------------------------------------------------------------
        // IInteger implementation
        //-------------------------------------------------------------

        //! Set feature value
        virtual void InternalSetValue(int64_t Value, bool Verify = true);

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
            CONVERT_ENUM_ENTRY(Sign, m_Sign, ESignClass)
            CONVERT_ENUM_ENTRY(Endianess, m_Endianess, EEndianessClass)
            CONVERT_ENUM_ENTRY(Representation, m_Representation, ERepresentationClass)
            CHAIN_CONVERT_MAP(CRegisterImpl)
        END_CONVERT_MAP

    protected:
        //! helpers to handle the register access incl. swapping
        void ReadReg(uint8_t* ValueBytes);
        
        //! helpers to handle the register access incl. swapping
        void WriteReg(uint8_t* ValueBytes);    
        
        //-------------------------------------------------------------
        // Member variables
        //-------------------------------------------------------------

        //! minimum value to be stored in the Register
        int64_t  m_Min;

        //! maximum value to be stored in the Register
        int64_t  m_Max;

        //! sign of the integer
        ESign m_Sign;

        //! representation of the integer
        ERepresentation  m_Representation;

        //! endianess of the register
        EEndianess m_Endianess;

        //! The bit indicating the sign of the number to be retrievend
        uint64_t m_SignBit;

        //! The bit to be set to -1 if the number is negative
        uint64_t m_SignMask;

    };

    //! implements the IntReg node
    class GENAPI_DECL CIntReg : public BaseT < ValueT < IntegerT < RegisterT < NodeT <  CIntRegImpl > > > > >
    {
    };
}

#pragma warning ( pop )

#endif // GENAPI_INTREG_H
