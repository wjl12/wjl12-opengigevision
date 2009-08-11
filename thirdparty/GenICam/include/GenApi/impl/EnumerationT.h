//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Alexander Happe
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/EnumerationT.h,v 1.9 2007/08/16 13:56:24 hartmut_nebelung Exp $
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
\brief    Definition of the EnumerationT class template
*/

#ifndef GENAPI_ENUMERATIONT_H
#define GENAPI_ENUMERATIONT_H

#include <GenApi/IEnumeration.h>

#ifdef _WIN32
#   define FMT_I64  "%I64d"
#else
#   define FMT_I64  "%lld"
#endif

namespace GenApi
{
    /** 
    \brief Implementation of the IEnumeration Interface
    */
    template<class Base>
    class GENAPI_DECL EnumerationT : public Base
    {
    public:
        //! Implementation of IEnumeration::GetSymbolics()
        virtual void GetSymbolics(StringList_t & Symbolics)
        {
            AutoLock l(Base::GetLock());
            return Base::InternalGetSymbolics(Symbolics);
        }

        //! Implementation of IEnumeration::GetEntries()
        virtual void GetEntries(NodeList_t & Entries)
        {
            AutoLock l(Base::GetLock());
            return Base::InternalGetEntries(Entries);
        }

        //! Implementation of IEnumeration::operator=()
        virtual IEnumeration& operator=(const GenICam::gcstring& ValueStr)
        {
            static_cast<IValue *>(this)->FromString(ValueStr);
            return *this;
        }

        //! Implementation of IEnumeration::SetIntValue()
        virtual void SetIntValue(int64_t Value, bool Verify = true)
        {
            AutoLock l(Base::GetLock());

            #ifdef LOG4C            
                Counter& c(Base::GetBathometer()); 
                if( 0 == c )
                    Base::m_pValueLog->info(">>>>" );

                Base::m_pValueLog->info("SetIntValue( " FMT_I64 " )", Value );
            #endif

            if( Verify )
            {
                 if( !IsWritable( this ) )
                        throw OUT_OF_RANGE_EXCEPTION("Node is not writable"); 
            }

            
            {
                typename Base::PostSetValueFinalizer PostSetValueCaller(this);  // dtor calls Base::PostSetValue

                Base::PreSetValue();
                Base::InternalSetIntValue(Value, Verify);
            
                // now Base::PostSetValue is called by ~PostSetValueFinalizer, i.e. updates are fired
                // if this is the call on the top of a chain of SetValue calls
            }

            #ifdef LOG4C            
                if( 0 == c )
                    Base::m_pValueLog->info("<<<<" );
            #endif
        }

        //! Implementation of IEnumeration::operator*()
        virtual GenICam::gcstring operator*()
        {
            return static_cast<IValue *>(this)->ToString();
        }

        //! Implementation of IEnumeration::GetIntValue()
        virtual int64_t GetIntValue(bool Verify = false)
        {
            AutoLock l(Base::GetLock());
            #ifdef LOG4C            
                Base::m_pValueLog->info("GetIntValue...");
            #endif
            int64_t IntValue = Base::InternalGetIntValue(Verify);
            #ifdef LOG4C            
                Base::m_pValueLog->info("...GetIntValue = " FMT_I64, IntValue );
            #endif
            return IntValue;

        }

        //! Implementation of IEnumeration::GetEntryByName()
        virtual IEnumEntry *GetEntryByName(const GenICam::gcstring& EntryName)
        {
            AutoLock l(Base::GetLock());
            return Base::InternalGetEntryByName(EntryName);
        }

    };

}
#endif
