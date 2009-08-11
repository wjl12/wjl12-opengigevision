//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Hartmut Nebelung
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/IntKey.h,v 1.10 2007/08/16 13:56:25 hartmut_nebelung Exp $
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
\brief    Definition of CIntKey
*/

#ifndef GENAPI_INTKEY_H
#define GENAPI_INTKEY_H

#include "Node.h"
#include "IEEE1212Parser.h"
#include "BaseT.h"
#include "ValueT.h"
#include "IntegerT.h"
#include "NodeT.h"

#pragma warning ( push )
#pragma warning ( disable : 4275 ) // non dll-interface XXX used as base for  dll-interface class YYY

namespace GenApi
{
    //*************************************************************
    // CIntKey class
    //*************************************************************
    class CIEEE1212Parser;
    /**
    * \ingroup internal_impl
    * \brief IInteger implementation for IEEE1212 integer entries
    */
    class GENAPI_DECL CIntKeyImpl : public IInteger, public CNodeImpl
    {
    public:
        //! Constructor
        CIntKeyImpl();

    private:
        //! not implemented: assignment operator
        CIntKeyImpl& operator=( const CIntKeyImpl& rhs );


    protected:
        //! determins the node's access mode
        virtual EAccessMode InternalGetAccessMode() const;

        //-------------------------------------------------------------
        // INodePrivate implementation
        //-------------------------------------------------------------

    public:
        //! Initializes the object
        virtual void FinalConstruct();

        //-------------------------------------------------------------
        // IInteger implementation
        //-------------------------------------------------------------

    protected:
        //! Set feature value
        #pragma BullseyeCoverage off
        virtual void InternalSetValue(int64_t /*Value*/, bool /* Verify = true*/ )
        {
            //assert(false);
            throw ACCESS_EXCEPTION("IntKey : %s SetValue failed. IntKey is read only", m_Name.c_str() );   
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
            CONVERT_ENTRY(Key, m_Key)
            CONVERT_NODE_REFERENCE( p1212Parser, m_pParser, CIEEE1212Parser )
            CHAIN_CONVERT_MAP(CNodeImpl)
        END_CONVERT_MAP

    protected:
        // inititialize when childs are initialized
        //-------------------------------------------------------------
        // Memeber variables
        //-------------------------------------------------------------

        //! minimum value
        const int64_t  m_Min;

        //! maximum value
        const int64_t  m_Max;

        //! Key of IEEE1212 Configuration Rom entry
        uint16_t m_Key;

        //! most recent Value
        int64_t m_Value;

        //! The config rom reader
        CIEEE1212Parser *m_pParser;

        //! representation of value
        const ERepresentation m_Representation;

    };

    class GENAPI_DECL CIntKey : public BaseT< ValueT< NodeT< IntegerT< CIntKeyImpl > > > >
    {
    private:
        //! not implemented: assignment operator
        CIntKey& operator=( const CIntKey& rhs );
    };

}

#pragma warning ( pop )

#endif // GENAPI_INTKEY_H
