//{{NO_DEPENDENCIES}}
//@tab=(8,4)

//-----------------------------------------------------------------------------
//  (c) 2007 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenICam
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/Base/GCLinkage.h,v 1.2.2.1 2008/07/14 08:37:44 hartmut_nebelung Exp $
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
\brief    helpers for pragma linkage
*/

#ifndef LINKAGE_H
#define LINKAGE_H

#if defined (_MSC_VER)

#   if (defined (WIN32) || defined (_WIN32)) && defined (_MT )
#       if defined(_DEBUG)
#           define CONFIGURATION "MDd"
#       elif defined(NDEBUG)
#           define CONFIGURATION "MD"
#       else
#           error Invalid configuraiton
#       endif
#   else
#       error Invalid configuration
#   endif

#   if !defined(COMPILER) // COMPILER  may be force set from outside
#       if _MSC_VER==1310  // Visual C++ .NET 2003
#           define COMPILER "VC71"
#       elif _MSC_VER==1400  // Visual C++ 2005
#           define COMPILER "VC80"
#       else
#           error Invalid compiler
#       endif
#   endif

#   define GENICAM_SUFFIX( CONFIGURATION, COMPILER, VERSION_MAJOR, VERSION_MINOR, EXTENSION ) \
        "_" CONFIGURATION "_" COMPILER "_v" VERSION_MAJOR "_" VERSION_MINOR "." EXTENSION 

#   define LIB_SUFFIX \
        GENICAM_SUFFIX( CONFIGURATION, COMPILER, GENICAM_VERSION_MAJOR_STR, GENICAM_VERSION_MINOR_STR, "lib" )
#   define DLL_SUFFIX \
        GENICAM_SUFFIX( CONFIGURATION, COMPILER, GENICAM_VERSION_MAJOR_STR, GENICAM_VERSION_MINOR_STR, "dll" )
#   define EXE_SUFFIX \
        GENICAM_SUFFIX( CONFIGURATION, COMPILER, GENICAM_VERSION_MAJOR_STR, GENICAM_VERSION_MINOR_STR, "exe" )

#elif defined (__GNUC__) && defined (__ELF__)
#   if defined (NDEBUG)
#       define CONFIGURATION ""
#   else
#       define CONFIGURATION "_g"
#   endif

#   define GENICAM_SUFFIX( CONFIGURATION, VERSION_MAJOR, VERSION_MINOR, EXTENSION ) \
        CONFIGURATION "." VERSION_MAJOR "." VERSION_MINOR "." EXTENSION

#   define LIB_SUFFIX \
        GENICAM_SUFFIX( CONFIGURATION, GENICAM_VERSION_MAJOR_STR, GENICAM_VERSION_MINOR_STR, "lib" )
#   define DLL_SUFFIX \
        GENICAM_SUFFIX( CONFIGURATION, GENICAM_VERSION_MAJOR_STR, GENICAM_VERSION_MINOR_STR, "dll" )
#   define EXE_SUFFIX \
        GENICAM_SUFFIX( CONFIGURATION, GENICAM_VERSION_MAJOR_STR, GENICAM_VERSION_MINOR_STR, "exe" )

#else
#   error Unknown shared library support
#endif

#define LIB_NAME( MODULE ) \
     MODULE ## LIB_SUFFIX

#define DLL_NAME( MODULE ) \
     MODULE ## DLL_SUFFIX

#define EXE_NAME( MODULE ) \
     MODULE ## EXE_SUFFIX

#endif // LINKAGE_H
