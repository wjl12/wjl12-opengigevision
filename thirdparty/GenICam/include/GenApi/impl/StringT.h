//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Alexander Happe
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/StringT.h,v 1.9 2007/08/16 13:56:26 hartmut_nebelung Exp $
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
\brief    Definition of the NodeT class template
*/

#ifndef GENAPI_STRINGT_H
#define GENAPI_STRINGT_H

#include "../IString.h"

namespace GenApi
{

    /**
    \brief Implementation of the IString interface
    */
    template< class Base >
    class GENAPI_DECL StringT : public Base
    {
    public:
        //! Set node value
        virtual void SetValue(const GenICam::gcstring& Value, bool Verify = true)
        {
            AutoLock l(Base::GetLock());

            #ifdef LOG4C    
                Counter& c(Base::GetBathometer()); 
                if( 0 == c )
                    Base::m_pValueLog->info(">>>>" );

                Base::m_pValueLog->info("SetValue = '%s' ", Value.c_str() );
            #endif

            {
                typename Base::PostSetValueFinalizer PostSetValueCaller(this);  // dtor calls Base::PostSetValue

                Base::PreSetValue(); // invalidates all nodes if this is the first call in a chain of SetValue calls
                Base::InternalSetValue(Value, Verify);

                // now Base::PostSetValue is called by ~PostSetValueFinalizer, i.e. updates are fired
                // if this is the call on the top of a chain of SetValue calls
            }

            #ifdef LOG4C            
                if( 0 == c )
                    Base::m_pValueLog->info("<<<<" );
            #endif

        }

        //! Set node value
        virtual IString& operator=(const GenICam::gcstring& Value)
        {
            SetValue(Value);
            return *this;
        }

        //! Get node value
        virtual GenICam::gcstring GetValue(bool Verify = false)
        {
            AutoLock l(Base::GetLock());
            
            #ifdef LOG4C            
                Base::m_pValueLog->info("GetValue...");
            #endif
            GenICam::gcstring ValueStr(Base::InternalGetValue(Verify));
            #ifdef LOG4C            
                Base::m_pValueLog->info("...GetValue = %s", ValueStr.c_str() );
            #endif
            return ValueStr;

        }

        //! Get node value
        virtual GenICam::gcstring operator()()
        {
            return GetValue();
        }

        //! Get node value
        virtual GenICam::gcstring operator*()
        {
            return GetValue();
        }

    };

}

#endif // GENAPI_STRINGT_H
