//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/Enumeration.h,v 1.16.2.1 2008/07/14 08:45:05 hartmut_nebelung Exp $
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
\brief    Definition of CEnumeratorBase and CEnumeration_Int
*/

#ifndef GENAPI_ENUMERATION_H
#define GENAPI_ENUMERATION_H

#include <Base/GCStringVector.h>
#include "Node.h"
#include "EnumDelegate.h"
#include "../Pointer.h"
#include "BaseT.h"
#include "ValueT.h"
#include "EnumerationT.h"

#pragma warning ( push )
#pragma warning ( disable : 4275 ) // non dll-interface XXX used as base for  dll-interface class YYY

namespace GenApi
{

    //*************************************************************
    // CEnumeration class
    //*************************************************************

    class GENAPI_DECL CEnumerationImpl : public CNodeImpl, public IEnumeration
    {
    public:
        //! Constructor
        CEnumerationImpl() : 
            m_pInteger(NULL)
        {};
#if 0
	private:
        CEnumerationImpl( const CEnumerationImpl& );
		CEnumerationImpl& operator= ( const CEnumerationImpl& );
#endif 
    protected:
        //! Get the enumeration map
        virtual void InternalGetSymbolics(StringList_t& Symbolics) 
        {
            m_EnumDelegate.GetSymbolics(Symbolics);
        };

        //! Get list of entry nodes
        virtual void InternalGetEntries(NodeList_t & Entries)
        {
            m_EnumDelegate.GetEntries(Entries);
        };

        virtual EAccessMode InternalGetAccessMode() const
        {
            return CNodeImpl::InternalGetAccessMode(m_pInteger);
        }

        //! Gets the symbolic value
        virtual GenICam::gcstring InternalToString(bool Verify = false) 
        { 
            int64_t Value = GetIntValue(Verify);
            GenICam::gcstring ValueStr; 
            m_EnumDelegate.ToString(ValueStr,Value);
            
            return ValueStr; 
        }

        //! Sets from symbolic value
        virtual void InternalFromString(const GenICam::gcstring& ValueStr, bool Verify = true) 
        { 
            int64_t Value; 
            if (!m_EnumDelegate.FromString(ValueStr,Value)) 
                throw INVALID_ARGUMENT_EXCEPTION("Feature '%s' : cannot convert value '%s', the value is invalid.", m_Name.c_str(),  ValueStr.c_str() ); 

            SetIntValue(Value, Verify); 
        } 

        //! Sets the integer value
        virtual void InternalSetIntValue(int64_t Value, bool Verify = true) 
        { 
            if ( Verify )
            {
                IEnumEntry* pEntry = m_EnumDelegate.IntValueToEntry(Value);
                if ( ! pEntry )
                    throw INVALID_ARGUMENT_EXCEPTION(" invalid enum value '%ld'", Value);

                if ( ! IsWritable(pEntry) )
                    throw ACCESS_EXCEPTION("Failed to write enumeration value. Access mode is not RW or WO");
            }

            if (m_pInteger)
            {
                m_pInteger->SetValue(Value, Verify); 
            } 
            else
            {
                m_Value = Value;
            }
        } 

        //! Gets the integer value
        virtual int64_t InternalGetIntValue(bool Verify = false) 
        { 
            if (m_pInteger) 
            {
                return m_pInteger->GetValue(Verify); 
            } 
            else
            {
                if (Verify)
                {
                    IEnumEntry* pEntry = m_EnumDelegate.IntValueToEntry(m_Value);
                    if ( ! pEntry )
                        throw OUT_OF_RANGE_EXCEPTION(" invalid enum value '%ld'", m_Value);
                }
                return m_Value;
            }
        }

        
        //! Get an entry node by name
        virtual IEnumEntry *InternalGetEntryByName(const GenICam::gcstring& EntryName)
        {
            NodeList_t Entries;
            GetEntries(Entries);
            
            NodeList_t::iterator pEntry;
            for(pEntry = Entries.begin(); pEntry != Entries.end(); pEntry++ )
            {
                CEnumEntryPtr ptrEnumEntry = *pEntry;
                assert(ptrEnumEntry.IsValid());

                if( EntryName == ptrEnumEntry->GetSymbolic() )
                {
                    return ptrEnumEntry;
                }
            }
            return NULL;

        }

    public:
        virtual bool IsTerminalNode() const { return m_pInteger == NULL; }

        virtual void FinalConstruct()
        {
            CNodeImpl::FinalConstruct();
            if ( m_pInteger )
                m_TerminalCandidates.insert(dynamic_cast<INodePrivate*>(m_pInteger));
        }

        //! Convert map
        BEGIN_CONVERT_MAP
            ADD_ENUM_ENTRY(pEnumEntry,(&m_EnumDelegate))
            CONVERT_NODE_REFERENCE(pValue, m_pInteger, IInteger)
            CONVERT_ENTRY(Value, m_Value)
            CHAIN_CONVERT_MAP(CNodeImpl)
        END_CONVERT_MAP

    protected:
        //-------------------------------------------------------------
        // Member variables
        //-------------------------------------------------------------

        //! Pointer to node providing access to the integer equivalent of the enum
        IInteger *m_pInteger;

        //! the class handling the enum conversion
        CEnumDelegate m_EnumDelegate;

        //! Holds the value if no register/IInteger is provided
        int64_t m_Value;
    };

    class GENAPI_DECL CEnumeration : public BaseT< ValueT< EnumerationT < NodeT < CEnumerationImpl> > > >
    {
#if 1
	public:
		CEnumeration() : 
		BaseT< ValueT< EnumerationT < NodeT < CEnumerationImpl> > >  >()
		{}
	private:
		CEnumeration( const CEnumeration& );
		CEnumeration& operator= ( const CEnumeration& );
#endif
    };

}

#pragma warning ( pop )

#endif // ifndef GENAPI_ENUMERATION_H
