//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenICam
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenICamVersion.h,v 1.1 2007/11/14 12:35:43 hartmut_nebelung Exp $
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
\brief    central versioning counters 
*/

#ifndef GENICAM_VERSION_H
#define GENICAM_VERSION_H


// Includes a machine generated file containing the version and build number
// The version numbers come from $(GENICAM_ROOT)/SetGenICamVersion.cmd
#include "_GenICamVersion.h"

// Don't ask...
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

// String versions of the version numbers 
#define GENICAM_VERSION_MAJOR_STR           TOSTRING( GENICAM_VERSION_MAJOR )
#define GENICAM_VERSION_MINOR_STR           TOSTRING( GENICAM_VERSION_MINOR )
#define GENICAM_VERSION_SUBMINOR_STR        TOSTRING( GENICAM_VERSION_SUBMINOR )
#define GENICAM_VERSION_BUILD_STR           TOSTRING( GENICAM_VERSION_BUILD )
#define GENICAM_ROOT_VERSION                "GENICAM_ROOT_V" GENICAM_VERSION_MAJOR_STR "_" GENICAM_VERSION_MINOR_STR

#endif // GENICAM_VERSION_H
