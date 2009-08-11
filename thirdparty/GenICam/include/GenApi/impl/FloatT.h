//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Alexander Happe
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/FloatT.h,v 1.11.2.1 2008/07/14 08:20:35 hartmut_nebelung Exp $
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
\brief    Definition of the FloatT class template
*/

#ifndef GENAPI_FLOATT_H
#define GENAPI_FLOATT_H

#include "INodePrivate.h"
#include "../Synch.h"
#include "../Counter.h"
#include "Value2String.h"

namespace GenApi
{
    /** 
    \brief Implementation of the IFloat Interface
    */
    template<class Base>
    class GENAPI_DECL FloatT : public Base
    {
    public:

        //! Implementation of IFloat::SetValue()
        virtual void SetValue(double Value, bool Verify = true) 
        {
            AutoLock l(Base::GetLock());

            #ifdef LOG4C            
                Counter& c(Base::GetBathometer()); 
                if( 0 == c )
                    Base::m_pValueLog->info(">>>>" );

                if( Verify )
                    Base::m_pValueLog->info("SetValue( %f, true )", Value );
                else
                    Base::m_pValueLog->info("SetValue( %f, false )", Value );
            #endif


            if( Verify && !IsWritable( this ) )
                throw ACCESS_EXCEPTION("Node is not writable"); 

            {

                typename Base::PostSetValueFinalizer PostSetValueCaller(this);  // dtor calls Base::PostSetValue


                // invalidates all nodes if this is the first call in a chain of SetValue calls
                Base::PreSetValue(); 

                if( Verify )
                {
                    CHECK_RANGE_FLT(Value, Base::InternalGetMin(), Base::InternalGetMax());
                }
            
                // sets the value
                Base::InternalSetValue(Value, Verify);
            
                #if ! defined( DISABLE_VALUE_CACHING ) || (DISABLE_VALUE_CACHING == 0)            
                    // Fill cache
                    if( WriteThrough == static_cast<INode *>(this)->GetCachingMode() )
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

        //! Implementation of IFloat::operator=()
        virtual IFloat& operator=(double Value) 
        {
            SetValue(Value);

            return *this;
        }

        //! Get value of the node as string
        virtual GenICam::gcstring InternalToString(bool Verify = false)
        {
            double value = GetValue(Verify);
            GenICam::gcstring valueString;
            Value2String(value, valueString);

            return valueString;
        }

        //! Implementation of IFloat::GetValue()
        virtual double GetValue(bool Verify = false) 
        {
            AutoLock l(Base::GetLock());

            if( Verify && !IsReadable( this ) )
                throw ACCESS_EXCEPTION("Node is not readable");

            #if ! defined( DISABLE_VALUE_CACHING ) || (DISABLE_VALUE_CACHING == 0)            
                // Use cache
                if (Base::m_ValueCacheValid && !Verify)
                {
                    #ifdef LOG4C            
                        Base::m_pValueLog->info("GetValue( false ) = %f  (from cache)", m_ValueCache );
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
            }

            const double Value(Base::InternalGetValue(Verify));
            
            #ifdef LOG4C            
                    Base::m_pValueLog->info("...GetValue = %f", Value );
            #endif

            if( Verify )
            {
                CHECK_RANGE_FLT(Value, Base::InternalGetMin(), Base::InternalGetMax());
            }

            #if ! defined( DISABLE_VALUE_CACHING ) || (DISABLE_VALUE_CACHING == 0)            
                // Fill cache
                const ECachingMode CachingMode(static_cast<INode *>(this)->GetCachingMode());
                if(    WriteThrough == CachingMode 
                    || WriteAround  == CachingMode  )
                {
                    m_ValueCache = Value;
                    Base::m_ValueCacheValid = true;
                }
            #endif

            return Value;

        }

        //! Implementation of IFloat::operator()()
        virtual double operator()() 
        {
            return GetValue();
        }

        //! Implementation of IFloat::operator*()
        virtual double operator*() 
        {
            return GetValue();
        }

        //! Set value of the node as string
        virtual void InternalFromString(const GenICam::gcstring& ValueStr, bool Verify = true)
        {
            double value;
            if (!String2Value(ValueStr,&value))
                throw INVALID_ARGUMENT_EXCEPTION("Node '%s' : cannot convert string '%s' to double", Base::m_Name.c_str(),  ValueStr.c_str() );

            SetValue(value, Verify);
        }


        //! Implementation of IFloat::GetMin()
        virtual double GetMin() 
        {
            AutoLock l(Base::GetLock());

            #ifdef LOG4C            
                Base::m_pRangeLog->info("GetMin...");
            #endif

            double Minimum = Base::InternalGetMin();
            
            #ifdef LOG4C            
                Base::m_pRangeLog->info("...GetMin = %f", Minimum );
            #endif
            
            return Minimum;

        }

        //! Implementation of IFloat::GetMax()
        virtual double GetMax() 
        {
            AutoLock l(Base::GetLock());

            #ifdef LOG4C            
                Base::m_pRangeLog->info("GetMax...");
            #endif
            
            const double Maximum(Base::InternalGetMax());
            
            #ifdef LOG4C            
                Base::m_pRangeLog->info("...GetMax = %f", Maximum );
            #endif
            
            return Maximum;

        }

        //! Implementation of IFloat::GetRepresentation()
        virtual  ERepresentation GetRepresentation() 
        {
            AutoLock l(Base::GetLock());
            return Base::InternalGetRepresentation();
        }

        //! Implementation of IFloat::GetUnit()
        virtual GenICam::gcstring GetUnit() const 
        {
            AutoLock l(Base::GetLock());
            return Base::InternalGetUnit();
        }

    protected:
        //! The cache for the float value
        double m_ValueCache;
    };
}

#endif // GENAPI_FLOATT_H
