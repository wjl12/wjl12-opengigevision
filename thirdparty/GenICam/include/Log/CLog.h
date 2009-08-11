//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  GenApi
//    Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/Log/CLog.h,v 1.7 2007/08/16 13:56:43 hartmut_nebelung Exp $
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
/**
\file
\brief    Implementation of CLog.
\ingroup Log_PublicUtilities
*/

#ifndef DEF_CLOG_H_
#define DEF_CLOG_H_

#pragma warning (push, 3)
    #include <log4cpp/Category.hh>
    #include <log4cpp/Appender.hh>
    #include <log4cpp/FileAppender.hh>
    #include <log4cpp/OstreamAppender.hh>
    #include <log4cpp/Layout.hh>
    #include <log4cpp/BasicLayout.hh>
    #include <log4cpp/Priority.hh>
    #include <log4cpp/NDC.hh>
    #include <log4cpp/PatternLayout.hh>
#if defined (_WIN32)
    #include <log4cpp/Win32DebugAppender.hh>
    #include <io.h>
#elif defined (__GNUC__) && defined (__unix__)
    #include <log4cpp/SyslogAppender.hh>
#endif
    #include <log4cpp/PropertyConfigurator.hh>
#pragma warning (pop)

#include "LogDll.h"
#include <stdio.h>
#include <Base/GCBase.h>

    
namespace GenICam
{

    /**
    \brief Helper class encapsualting log4cpp
    \ingroup Log_PublicUtilities
    */
    class LOG_DECL CLog
    {
    public:
        //! Retrieves the root category
        static log4cpp::Category& GetRootLogger( void );
        
        //! REtrieves (and if necessary create) a category by name
        static log4cpp::Category& GetLogger( const GenICam::gcstring &LoggerName );

        //! Push nested diagnostic context
        static void PushNDC( const GenICam::gcstring &ContextName );

        //! Pop nested diagnostic context
        static void PopNDC( void );

        //! initilizes log4cpp
        static void Initialize( void );
        
        //! de-initializes log4cpp
        static void ShutDown( void );

        //! Configures log4cpp to output messages >=ERROR on the Windows debug console
        static void ConfigureDefault();

        //! Configures log4cpp from a file
        static bool ConfigureFromFile( const GenICam::gcstring &FileName );

        //! Configures log4cpp from a file whose name is given by the environment variable GENICAM_LOG_CONFIG
        static bool ConfigureFromEnvironment( void );

        //! Configures log4cpp from a string
        static bool ConfigureFromString( const GenICam::gcstring &ConfigData );
    };

}

#endif // DEF_CLOG_H_
