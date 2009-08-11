//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Alexander Happe
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/IntegerT.h,v 1.12.2.1 2008/07/14 08:20:35 hartmut_nebelung Exp $
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
\brief    Definition of the IntegerT class template
*/

#ifndef GENAPI_INTEGERT_H
#define GENAPI_INTEGERT_H

#include "../IInteger.h"

#ifdef _MSC_VER
#   define FMT_I64  "%I64d"
#else
#   define FMT_I64  "%lld"
#endif

namespace GenApi
{

    /** 
    \brief Implementation of the IInteger Interface
    */
    template <class Base> 
    class GENAPI_DECL IntegerT 
        : public Base
    {
    public:
        //! Implementation of IInteger::SetValue
        virtual void SetValue(int64_t Value, bool Verify = true)
        {
            AutoLock l(Base::GetLock());

            Base::m_ValueCacheValid = false;


            #ifdef LOG4C            
                Counter& c(Base::GetBathometer()); 
                if( 0 == c )
                    Base::m_pValueLog->info(">>>>" );

                Base::m_pValueLog->info("SetValue( " FMT_I64 ", %s )", Value, Verify ? "true" : "false" );
            #endif


            if( Verify && !IsWritable( this ) )
                throw  ACCESS_EXCEPTION("Node is not writable");

            {
                typename Base::PostSetValueFinalizer PostSetValueCaller(this);  // dtor calls Base::PostSetValue

                // invalidates all nodes if this is the first call in a chain of SetValue calls        
                Base::PreSetValue(); 

                if( Verify )
                {
                    assert(0 != Base::InternalGetInc());
                    CHECK_RANGE_I64( Value, Base::InternalGetMin(), Base::InternalGetMax(), Base::InternalGetInc() );
                }

                // sets the value
                Base::InternalSetValue(Value, Verify);

                #if ! defined( DISABLE_VALUE_CACHING ) || (DISABLE_VALUE_CACHING == 0)            
                    // Fill cache
                    if(
                        WriteThrough ==
                            static_cast<INode *>(this)->GetCachingMode())
                    {
                        m_ValueCache = Value;
                        Base::m_ValueCacheValid = true;
                    }
                #endif 

                // now Base::PostSetValue is called by ~PostSetValueFinalizer, i.e. updates are fired
                // if this is the call on the top of a chain of SetValue calls
            }

            #ifdef LOG4C            
                if( 0 == c )
                    Base::m_pValueLog->info("<<<<" );
            #endif
        }

        //! Implementation of IInteger::operator=()
        virtual IInteger& operator=(int64_t Value)
        {
            SetValue(Value);
            return *this;
        }

        void InternalFromString(const GenICam::gcstring& ValueStr, bool Verify = true)
        {
            int64_t Value;
            if (!String2Value(ValueStr, &Value))
                throw INVALID_ARGUMENT_EXCEPTION("Node '%s' : cannot convert string '%s' to int", Base::m_Name.c_str(),  ValueStr.c_str() );

            SetValue(Value, Verify);
        }

        //! Implementation of IInteger::GetValue()
        virtual int64_t GetValue(bool Verify = false)
        {
            AutoLock l(Base::GetLock());

            #if ! defined( DISABLE_VALUE_CACHING ) || (DISABLE_VALUE_CACHING == 0)            
                // Use cache
                if ( Base::m_ValueCacheValid && !Verify)
                {
                    #ifdef LOG4C            
                        Base::m_pValueLog->info("GetValue( false ) = " FMT_I64 "  (from cache)", m_ValueCache );
                    #endif
                    return m_ValueCache;
                }
            #endif

            #ifdef LOG4C            
                if( Verify )
                    Base::m_pValueLog->info("GetValue( true )...");
                else
                    Base::m_pValueLog->info("GetValue( false )...");
            #endif

            if( Verify )
            {
                SetInvalid(Base::simOnlyMe);
                if( !IsReadable( this ) )
                    throw ACCESS_EXCEPTION("Node is not readable"); \
            }

            const int64_t Value(Base::InternalGetValue(Verify));
            
            #ifdef LOG4C            
                Base::m_pValueLog->info("...GetValue = " FMT_I64, Value );
            #endif

            if( Verify )
            {
                CHECK_RANGE_I64( Value, Base::InternalGetMin(), Base::InternalGetMax(), Base::InternalGetInc() );
            }

            #if ! defined( DISABLE_VALUE_CACHING ) || (DISABLE_VALUE_CACHING == 0)            
                // Fill cache
                const ECachingMode
                    CachingMode(static_cast<INode *>(this)->GetCachingMode());
                if(    WriteThrough == CachingMode 
                    || WriteAround  == CachingMode  )
                {
                    m_ValueCache = Value;
                    Base::m_ValueCacheValid = true;
                }
            #endif

            return Value;
        }

        //! Implementation of IInteger::operator()()
        virtual int64_t operator()()
        {
            return GetValue();
        }

        //! Implementation of IInteger::operator*()
        virtual int64_t operator*()
        {
            return GetValue();
        }

        GenICam::gcstring InternalToString(bool Verify = false)
        {
            int64_t Value = GetValue(Verify);

            GenICam::gcstring ValueStr;
            Value2String(Value, ValueStr);
            
            return ValueStr;
        }

        //! Implementation of IInteger::GetMin()
        virtual int64_t GetMin()
        {
            AutoLock l(Base::GetLock());

            #ifdef LOG4C            
                Base::m_pRangeLog->info("GetMin...");
            #endif

            int64_t Minimum = Base::InternalGetMin();

            #ifdef LOG4C            
                Base::m_pRangeLog->info("...GetMin = " FMT_I64, Minimum );
            #endif

            return Minimum;

        }

        //! Implementation of IInteger::GetMax()
        virtual int64_t GetMax()
        {
            AutoLock l(Base::GetLock());

            #ifdef LOG4C            
                Base::m_pRangeLog->info("GetMax...");
            #endif

            int64_t Maximum = Base::InternalGetMax();
            
            #ifdef LOG4C            
                Base::m_pRangeLog->info("...GetMax = " FMT_I64, Maximum );
            #endif
            
            return Maximum;
        }

        //! Implementation of IInteger::GetInc()
        virtual int64_t GetInc()
        {
            AutoLock l(Base::GetLock());

            #ifdef LOG4C            
                Base::m_pRangeLog->info("GetInc...");
            #endif

            const int64_t Increment(Base::InternalGetInc());

            #ifdef LOG4C            
                Base::m_pRangeLog->info("...GetInc = " FMT_I64, Increment );
            #endif
            
            return Increment;
        }

        //! Implementation of IInteger::GetRepresentation
        virtual  ERepresentation GetRepresentation()
        {
            AutoLock l(Base::GetLock());

            return Base::InternalGetRepresentation();
        }

        protected:
            //! The cache for the integer value
            int64_t m_ValueCache;
    };

} // namespace GenApi

#endif 
