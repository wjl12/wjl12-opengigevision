
//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Hartmut Nebelung
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/SmartFeature.h,v 1.9 2007/08/16 13:56:26 hartmut_nebelung Exp $
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
\brief    Interface to the CSmartFeature class.
*/

#ifndef GENAPI_SMARTFEATURE_H
#define GENAPI_SMARTFEATURE_H
#if _MSC_VER>1000
#pragma once
#endif // _MSC_VER>1000

#include "../Types.h"
#include "../IInteger.h"
#include "Register.h"
#include "BaseT.h"
#include "ValueT.h"
#include "NodeT.h"
#include "IntegerT.h"


//! Part of the generic device API
namespace GenApi
{
#pragma warning( push )
#pragma warning( disable:4275 )
    // *************************************************************
    // CSmartFeature class
    // *************************************************************
    //! Implements the Basler Smart Feature class
    /**
    * Intention is to use this class in pIsAvailable and pIsImplemented 
    * elements where the GetValue method is called in a boolean sense.
    * But also to use it as an address register.
    * 
    */
    class GENAPI_DECL CSmartFeatureImpl : public CRegisterImpl, public IInteger
    {
    public:
        CSmartFeatureImpl(void);

    protected:
        //-------------------------------------------------------------
        // Implementation of IRegister
        //-------------------------------------------------------------
        //!\name GenApi::IRegister implementation
        //{
        // Write silently to Register
        virtual void InternalSet(const uint8_t *pBuffer, int64_t Length);
        
        // Read with private caching
        virtual void InternalGet(uint8_t *pBuffer, int64_t Length); 
        //}
        //-------------------------------------------------------------
        // Implementation of IInteger
        //-------------------------------------------------------------
        //!\name GenApi::IInteger implementation 
        //{
        // Set feature value 
        virtual void InternalSetValue(int64_t /*Value*/, bool /*Verify = true*/)
        {
        }

        // Get feature value
        virtual int64_t InternalGetValue(bool Verify = false);

        // Get minimum value allowed
        virtual int64_t InternalGetMin() const;

        // Get maximum value allowed
        virtual int64_t InternalGetMax() const;

        // Get increment
        virtual int64_t InternalGetInc() const;

        // Get recommended representation
        virtual  ERepresentation InternalGetRepresentation() const
        {
            return _UndefindedRepresentation;
        }
        //\}
        //-------------------------------------------------------------
        // Implementation of INode
        //-------------------------------------------------------------
        //! \name INode implementation
        // \{
        virtual EAccessMode InternalGetAccessMode() const;
        // \}

        //-------------------------------------------------------------
        // Implementation of IValue
        //-------------------------------------------------------------
        //! \name IValue implementation
        // \{

        //Get content of the node as string
        virtual GenICam::gcstring InternalToString(bool Verify = false);
        //Set content of the node as string
        virtual void InternalFromString(const GenICam::gcstring& ValueStr, bool Verify = true) const;

        // \}

        
        //-------------------------------------------------------------
        // Implementation of INodePrivate
        //-------------------------------------------------------------
        //! \name INodePrivate implementation
        // \{
        virtual bool IsTerminalNode() const;


        //-------------------------------------------------------------
        // Implementatin of CNodeImpl
        //-------------------------------------------------------------
        //\{ \n CNodeImpl overrides
        virtual void SetInvalid(INodePrivate::ESetInvalidMode simMode);
        //\}

    public:
        //-------------------------------------------------------------
        // Initializing
        //-------------------------------------------------------------
        BEGIN_CONVERT_MAP
            CONVERT_ENTRY(FeatureID, m_FeatureID)
            CHAIN_CONVERT_MAP(CRegisterImpl)
        END_CONVERT_MAP
    private:
        GUID m_FeatureID;
        uint64_t m_SmartFeatureAdr;
        bool m_Valid;
    };

    class CSmartFeature : public BaseT< ValueT< NodeT< IntegerT< RegisterT< CSmartFeatureImpl > > > > >
    {
    };
#pragma warning( pop )
}
#endif // GENAPI_SMARTFEATURE_H
