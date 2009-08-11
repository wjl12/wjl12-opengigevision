//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Hartmut Nebelung
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/Boolean.h,v 1.10 2007/08/16 13:56:23 hartmut_nebelung Exp $
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
\brief    Definition of CBoolean
*/

#ifndef GENAPI_BOOLEAN_H
#define GENAPI_BOOLEAN_H

#if defined (_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif // _MSC_VER > 1000

#include "../IBoolean.h"
#include "Node.h"
#include "BooleanT.h"
#include "ValueT.h"
#include "BaseT.h"
#include "NodeT.h"

#pragma warning ( push )
#pragma warning ( disable : 4275 ) // non dll-interface XXX used as base for  dll-interface class YYY

namespace GenApi
{

    //*************************************************************
    // CBoolean class
    //*************************************************************

    /*! 
    * \brief IBoolean implementation
    *
    * May be used as a toggle. The Values for On and Off may be chosen.
    * Supposed to work together with a IInteger node, e.g. CMaskedInt,
    * for setting the concrete values.
    */

    class GENAPI_DECL CBooleanImpl : public  IBoolean, public CNodeImpl
    {
    public:
        //! Constructor
        CBooleanImpl();


    protected:
        //  Get the access mode of the node
        virtual EAccessMode InternalGetAccessMode() const;
        // \}

        //-------------------------------------------------------------
        //! \name Implementation of GenApi::IBoolean interface
        //\{
        // Set node value
        virtual void InternalSetValue(bool Value);

        // Get node value
        virtual bool InternalGetValue() const;
        // \}

        //! \name Implementation of GenApi::IValue interface
        //\{
        // Get value of the node as string
        virtual GenICam::gcstring InternalToString(bool Verify = false);

        // Set value of the node as string
        virtual void InternalFromString(const GenICam::gcstring& valueString, bool Verify = true);
        //\}

    public:
        virtual bool IsTerminalNode() const { return m_pValue == NULL; }

        virtual void FinalConstruct();

        //-------------------------------------------------------------
        // Initializing
        //-------------------------------------------------------------
        BEGIN_CONVERT_MAP
            CONVERT_NODE_REFERENCE(pValue, m_pValue, IInteger)
            CONVERT_ENTRY(Value, m_Value)
            CONVERT_ENTRY(OnValue, m_OnValue)
            CONVERT_ENTRY(OffValue, m_OffValue)
            CHAIN_CONVERT_MAP(CNodeImpl)
        END_CONVERT_MAP

    protected:

        //-------------------------------------------------------------
        // Member variables
        //-------------------------------------------------------------

        //! placeholder if no register node used
        mutable bool m_Value;

        //! Value for On
        int64_t m_OnValue;
        //! Value for Off
        int64_t m_OffValue;
        //! Pointer to the associated register
        IInteger *m_pValue;
    };


    class GENAPI_DECL CBoolean : public BaseT< ValueT< BooleanT<  NodeT < CBooleanImpl> > > >
    {
    };

}

#pragma warning ( pop )

#endif // GENAPI_BOOLEAN_H
