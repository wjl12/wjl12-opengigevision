/****************************************************************************
 (c) 2004 by Basler Vision Technologies
 (c) 2005 STEMMER IMAGING

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


****************************************************************************/

 /// \file
 /// \brief    Platform-dependent type definitions
 /// \version  $Revision: 1.15.2.3 $
 /// \date     $Date: 2008/08/29 09:26:14 $
 /// \ingroup Base_PublicUtilities


#ifndef GENICAM_TYPES_H
#define GENICAM_TYPES_H

#if defined (__unix__)
#   define  __STDC_LIMIT_MACROS
#   define  __STDC_CONSTANT_MACROS
#   include <stddef.h>
#   include <inttypes.h>
#   include <stdint.h>
#elif defined (_WIN32)
  //! \addtogroup Base_PublicUtilities
  //! \{

// tag to perform 64 compatibility warnings checks on a type
#   if !defined(GC_W64)
#       if defined(_M_IX86) && _MSC_VER >= 1300
#           define GC_W64 __w64
#       else
#           define GC_W64
#       endif
#   endif

  //! makes int 8 portable across different platforms
  typedef char int8_t;
  //! makes char 8 portable across different platforms
  typedef char char8_t;
  //! makes unsigned char portable across different platforms
  typedef unsigned char uint8_t;
  //! makes char 8 portable across different platforms
  typedef unsigned char uchar8_t;

  //! makes short portable across different platforms
  typedef short int16_t;
  //! makes unsigned short portable across different platforms
  typedef unsigned short uint16_t;

  //! makes __int32 portable across different platforms
  typedef __int32 int32_t;
  //! makes unsigned int32 portable across different platforms
  typedef unsigned __int32 uint32_t;

  //! makes int64 portable across different platforms
  typedef __int64 int64_t;
  //! makes unsigned int64 portable across different platforms
  typedef unsigned __int64 uint64_t;

  //! use a universal platform dependend int
  typedef __int64 int_t;
  //! use a universal platform dependend unsigned int
  typedef unsigned __int64 uint_t;

  #ifndef _SIZE_T_DEFINED
    #ifdef _WIN64
      //! use a universal platform dependend unsigned int
      typedef unsigned __int64 size_t;
    #else
      //! use a universal platform dependend unsigned int
      typedef GC_W64 unsigned int size_t;
    #endif
    #define _SIZE_T_DEFINED
  #endif

  #ifndef _INTPTR_T_DEFINED
    #ifdef _WIN64
      //! use a universal platform dependend intptr_t
      typedef __int64 intptr_t;
    #else
      //! use a universal platform dependend intptr_t
      typedef GC_W64 int intptr_t;
    #endif
    #define _INTPTR_T_DEFINED
  #endif

  #ifndef _UINTPTR_T_DEFINED
    #ifdef _WIN64
      //! use a universal platform dependend uintptr_t
      typedef unsigned __int64 uintptr_t;
    #else
      //! use a universal platform dependend intptr_t
      typedef GC_W64 unsigned int uintptr_t;
    #endif
    #define _UINTPTR_T_DEFINED
  #endif
#else
#   error No platform-specific type definitions
#endif /* __unix__ */



#ifndef INT64_MAX
#   define INT64_MAX   0x7fffffffffffffffLL    /* maximum signed int64 value */
#endif

#ifndef INT64_MIN
#   define INT64_MIN   0x8000000000000000LL    /* minimum signed int64 value */
#endif

#ifndef UINT64_MAX
#   define UINT64_MAX  0xffffffffffffffffULL   /* maximum unsigned int64 value */
#endif

#ifndef INT32_MAX
#   define INT32_MAX   0x000000007fffffffLL    /* maximum signed int32 value */
#endif

#ifndef INT32_MIN
#   define INT32_MIN   0xffffffff80000000LL    /* minimum signed int32 value */
#endif

#ifndef UINT32_MAX
#   define UINT32_MAX  0x00000000ffffffffULL   /* maximum unsigned int32 value */
#endif

#ifndef INT8_MAX
#   define INT8_MAX    0x000000000000007fLL    /* maximum signed int8 value */
#endif

#ifndef INT8_MIN
#   define INT8_MIN    0xffffffffffffff80LL    /* minimum signed int8 value */
#endif

#ifndef UINT8_MAX
#   define UINT8_MAX   0x00000000000000ffULL   /* maximum unsigned int8 value */
#endif

//! \addtogroup Base_PublicUtilities
//! \{

// Utility Types
#if defined(__cplusplus)
//! 8 bit bool
typedef bool              bool8_t;
#else
typedef uint8_t           bool8_t;
#endif
//! universal pointer to 8 bit const char
typedef const char *      pcchar8_t;
//! universal pointer to 8 bit char
typedef char *            pchar8_t;
//! 32 bit floating point
typedef float             float32_t;
//! 64 bit floating point
typedef double            float64_t;

//! \}



// gc base import/export
#if defined(_MSC_VER)
#   ifdef GCBASE_EXPORTS
#       define GCBASE_API __declspec( dllexport )
#   else
#       define GCBASE_API __declspec( dllimport )
#   endif
#elif defined (__GNUC__)
#   if (__GNUC__ >= 4) && defined (__ELF__)
#       define GCBASE_API __attribute__((visibility("default")))
#   else // (__GNUC__ >= 4) && defined (__ELF__)
#       define GCBASE_API
#   endif // (__GNUC__ >= 4) && defined (__ELF__)
#else
#   error Unsupported compiler or platform
#endif



//! version
typedef struct 
{
    uint16_t Major;        //!> a is incompatible with b if a != b
    uint16_t Minor;        //!> a is incompatible b a > b
    uint16_t SubMinor;     //!> a is aways compatible with b
} Version_t;


// mark a variable as unused. This prevents unused parameter/ unused local variable warnings on warning level 4.
#if defined(_MSC_VER)
#   define GC_UNUSED(unused_var) (void)(unused_var)
#elif defined(__GNUC__)
#   define GC_UNUSED(unused_var) // no changes to unused params with GCC
#else
#   error Unsupported compiler or platform
#endif


#endif // GENICAM_TYPES_H
