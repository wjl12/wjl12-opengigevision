//-----------------------------------------------------------------------------
//  (c) 2005 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenICam
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/Base/GCUtilities.h,v 1.11.2.2 2008/12/05 17:44:20 hartmut_nebelung Exp $
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
\brief    GenICam common utilities
\ingroup Base_PublicUtilities
*/

#ifndef GENAPI_GENAPIUTILITIES_DEF_H_
#define GENAPI_GENAPIUTILITIES_DEF_H_

#if defined (_WIN32)
# include <windows.h>
#endif

#include "../GenICamVersion.h"

#include "GCTypes.h"
#include "GCString.h"
#include "GCException.h"
#include "GCLinkage.h"

namespace GenICam
{
    template<typename Td, typename Ts>
    inline Td INTEGRAL_CAST2( Ts s )
    {
        const Td d = static_cast<Td>( s );
        if ( !static_cast<Ts>( d ) == s ){
            RUNTIME_EXCEPTION("INTEGRAL_CAST failed");
        }
        return d;
    }
	template<typename T>
	inline T INTEGRAL_CAST( int64_t ll )
	{
		return INTEGRAL_CAST2<T, int64_t>( ll );
	}

    //! Retrieve the value of an environment variable
    //! \throw runtime_exception if not found
    GCBASE_API gcstring GetValueOfEnvironmentVariable( const gcstring &Name );
    //! Replaces $(ENVIRONMENT_VARIABLES) in a string and replace ' ' with %20
    //! \ingroup Base_PublicUtilities
    GCBASE_API void ReplaceEnvironmentVariables(gcstring &Buffer, bool ReplaceBlankBy20 = false);

    //! retrieve the root folder of the GenICam installation
    /*! This function retrieves the content of the environment variable GENICAM_ROOT_V1_2
        whereas the actual version numbers depend on the GenICam version used
    */
    GCBASE_API gcstring GetGenICamRootFolder(void);

    //! retrieves the bin folder of the GenICam installation
    inline gcstring GetGenICamDLLFolder(void)
    {
        gcstring DLLFolder(GetGenICamRootFolder());
#if defined (_MSC_VER)
#   if defined (_WIN64)
        DLLFolder += "\\Bin\\Win64_x64";
#   else
        DLLFolder += "\\Bin\\Win32_i86";
#   endif
#elif defined (__GNUC__)
#   if defined (__LP64__)
        DLLFolder += "/lib64";
#   else
        DLLFolder += "/lib";
#   endif
#else
#   error Unknown Platform
#endif

        return DLLFolder;
    }

    //! retrieves the full name of a module's DLL
    inline gcstring GetGenICamDLLName(const gcstring&  ModuleName )
    {
        gcstring  DLLName(ModuleName);
        DLLName += DLL_SUFFIX;
        return DLLName;
    }

    //! retrieves the full path of a module's DLL
    inline gcstring GetGenICamDLLPath(const gcstring&  ModuleName )
    {
        gcstring DLLPath(GetGenICamRootFolder());
#if defined (_MSC_VER)
#   if defined (_WIN64)
        DLLPath += "\\Bin\\Win64_x64\\";
#   else
        DLLPath += "\\Bin\\Win32_i86\\";
#   endif
#elif defined (__GNUC__)
#   if defined (__LP64__)
        DLLPath += "/lib64/";
#   else
        DLLPath += "/lib/";
#   endif
#else
#   error Unknown Platform
#endif
        DLLPath += GetGenICamDLLName( ModuleName );
        return DLLPath;
    }

}

#endif // GENAPI_GENAPIUTILITIES_DEF_H_
