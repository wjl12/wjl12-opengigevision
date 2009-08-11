//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/Compatibility.h,v 1.10.2.1 2008/08/18 15:29:38 hartmut_nebelung Exp $
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
    \brief    Definition of macros for cross-platform compatibility
*/

#ifndef GENAPI_COMPATIBILITY_H
#define GENAPI_COMPATIBILITY_H

#if defined (_MSC_VER) && defined (_WIN32)

#define NOMINMAX

#pragma comment( lib, "ws2_32.lib")
#include <winsock.h>

#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN    4321
#if defined( _M_IX86 ) || defined( _M_X64 ) // *JS*
#define __BYTE_ORDER __LITTLE_ENDIAN
#else
#error Unsupported architecture
#endif

#include <guiddef.h>

#pragma intrinsic(_byteswap_ushort)
#pragma intrinsic(_byteswap_ulong)
#pragma intrinsic(_byteswap_uint64)
#define bswap_16(s) _byteswap_ushort(s)
#define bswap_32(s) _byteswap_ulong(s)
#define bswap_64(s) _byteswap_uint64(s)

#define _DEFINE_GUID(name, l, w0, w1, c0, c1, c2, c3, c4, c5, c6, c7) \
	GUID name = { l, w0, w1, { c0, c1, c2, c3, c4, c5, c6, c7 } };

#elif defined (__GNUC__) && defined (__unix__)

#include <endian.h>
#include <string.h>
#include <netinet/in.h>
#include <byteswap.h>

#define _snprintf snprintf
#define _strtoi64 strtoll
#define _vsnprintf vsnprintf
#define _strdup strdup
#define _isnan isnan

// these definitions break the stl-base code and should not be used!
//#define min(x, y) (((x) < (y)) ? (x) : (y))
//#define mAX(x, y) (((x) > (y)) ? (x) : (y))

/*
 * Watch out!
 * In order not to break existing code, we have to duplicate the
 * broken MS Windows GUID handling. Never mix with code using the
 * rfc4122-compliant uuid framework implemented by libuuid!
 */

typedef struct _GUID {
    uint32_t  Data1;
    uint16_t  Data2;
    uint16_t  Data3;
    unsigned char  Data4[ 8 ];
} __attribute__ ((packed)) GUID;

#define _DEFINE_GUID(name, l, w0, w1, c0, c1, c2, c3, c4, c5, c6, c7)   \
    GUID name = { l, w0, w1, { c0, c1, c2, c3, c4, c5, c6, c7 } }

#define IsEqualGUID(a, b) (!memcmp(&a, &b, sizeof (GUID)))

inline bool
operator ==(const GUID &a, const GUID &b)
{
    return IsEqualGUID(a, b);
}

#else
#   error Unsupported platform
#endif

#endif // ifndef GENAPI_COMPATIBILITY_H
