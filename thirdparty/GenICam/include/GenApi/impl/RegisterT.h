//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Alexander Happe
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/RegisterT.h,v 1.9 2007/08/16 13:56:26 hartmut_nebelung Exp $
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
\brief    Definition of the RegisterT class template
*/

#ifndef GENAPI_REGISTERT_H
#define GENAPI_REGISTERT_H

#include "../IRegister.h"
namespace GenApi
{

    /**
        \brief Implementation of the IRegister interface
    */
    template <class Base> 
    class GENAPI_DECL RegisterT 
        : public Base
    {
    public:

        //! Implementation of IRegister::Set()
        virtual void Set(const uint8_t *pBuffer, int64_t Length)
        {
            AutoLock l(Base::GetLock());

            #ifdef LOG4C    
                Counter& c(Base::GetBathometer()); 
                if( 0 == c )
                    Base::m_pValueLog->info(">>>>" );

                {
                    static const char fmt[] =
#ifdef _MSC_VER
                        "Set( %I64d, 0x";
#else
                        "Set( %lld, 0x";
#endif
                    static const int BufferLen(256);
                    char _pBuffer[256];
                    int BufferLeft(_snprintf(_pBuffer, BufferLen, fmt, Length));

                    /* MANTIS 0000062 */
                    for(int i = 0; i < Length; i++)
                    {
                        const int n =_snprintf(_pBuffer + BufferLeft,
                                               BufferLen - BufferLeft,
                                               "%02X", (unsigned int) pBuffer[i]);

#ifdef _MSC_VER
                        if (n < 0)
#else
                        if (BufferLeft + n >= BufferLen)
#endif
                            break;

                        BufferLeft += n;
                    }

                    Base::m_pValueLog->info("%s )", _pBuffer);
                }
            #endif

            {
                typename Base::PostSetValueFinalizer PostSetValueCaller(this);  // dtor calls Base::PostSetValue

                Base::PreSetValue(); // invalidates all nodes if this is the first call in a chain of SetValue like calls
                Base::InternalSet(pBuffer, Length);

                // now Base::PostSetValue is called by ~PostSetValueFinalizer, i.e. updates are fired
                // if this is the call on the top of a chain of SetValue calls
            }




            #ifdef LOG4C            
                if( 0 == c )
                    Base::m_pValueLog->info("<<<<" );
            #endif

        }


        //! Implementation of IRegister::Get()
        virtual void Get(uint8_t *pBuffer, int64_t Length)
        {
            AutoLock l(Base::GetLock());

            #ifdef LOG4C            
                Base::m_pValueLog->info("Get...");
            #endif

            Base::InternalGet(pBuffer, Length);

            #ifdef LOG4C
                {
                    static const char fmt[] =
#ifdef _MSC_VER
                        "...Get( %I64d ) = 0x";
#else
                        "...Get( %lld ) = 0x";
#endif


                    static const int BufferLen(256);
                    char _pBuffer[256];
                    int BufferLeft(_snprintf(_pBuffer, BufferLen, fmt, Length));

                    /* MANTIS 0000062 */
                    for(int i = 0; i < Length; i++)
                    {
                        const int n = _snprintf(_pBuffer + BufferLeft,
                                                BufferLen - BufferLeft,
                                                "%02X", (unsigned int) pBuffer[i]);
#ifdef _MSC_VER
                        if (n < 0)
#else
                        if (BufferLeft + n >= BufferLen)
#endif
                            break;

                        BufferLeft += n;
                    }
                    Base::m_pValueLog->info("%s", _pBuffer);
                }
            #endif
        }

        //! Implementation of IRegister::GetLength()
        virtual int64_t GetLength()
        {
            AutoLock l(Base::GetLock());

            return Base::InternalGetLength();
        }

        //! Retrieves the Address of the register 
        virtual int64_t GetAddress()
        {
            AutoLock l(Base::GetLock());

            return Base::InternalGetAddress();
        }

    };

} // namespace GenApi

#endif // GENAPI_REGISTERT_H
