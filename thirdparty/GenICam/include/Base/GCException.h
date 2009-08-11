//-----------------------------------------------------------------------------
//  (c) 2004 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenICam
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/Base/GCException.h,v 1.15 2007/08/16 13:56:44 hartmut_nebelung Exp $
//  
//  14.03.2005 Stemmer, RS 
//             changed minor things like namespace
//             added a AccessException
//             added a TimeoutException
//
//  21.02.2006 Stemmer, SD
//             used _vsnprintf_s function for VS8 and higher (compiler fork)
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
              Standard GenICam Exceptions
    \ingroup Base_PublicUtilities
*/

#ifndef GENCAM_EXCEPTION_H_
#define GENCAM_EXCEPTION_H_

#include <exception>
#include <cassert>
#include <cstdarg>
#include <sstream>
#include <stdio.h>
#include <stdarg.h>
#include "GCTypes.h"
#include "GCString.h"

#pragma pack(push, 8)

namespace GenICam
{

    /**
    \brief GenICam's exception class
    \ingroup Base_PublicUtilities
    */
    class GCBASE_API GenericException : public std::exception
    {
    public:
        //! Constructor
        GenericException( const char* description, const char *sourceFileName, unsigned int sourceLine );

        //! Get error description
        const char*
        GetDescription() const throw();

        //! Get filename in which the error occurred
        const char*
        GetSourceFileName() const throw();

        //! Get line number at which the error occurred
        unsigned int
        GetSourceLine() const throw();

        //! Get error description (overwrite from std:exception)
        virtual const char *what() const throw();

        virtual ~GenericException() throw();

    protected:
        //! Line number at which the error occurred
        unsigned int m_SourceLine;

        //! Filename in which the error occurred
        gcstring m_SourceFileName;

        //! Error description
        gcstring m_Description;

        //! Complete error message, including file name and line number
        gcstring m_What;

    };

    //! Creates an exception with the same functionality as the GenericException but being of different type
    //! \ingroup Base_PublicUtilities
    #define DECLARE_EXCEPTION( name ) \
        class GCBASE_API name : public GenICam::GenericException \
        { \
        public: \
            name(const char* description, const char *pSourceFileName, int SourceLine ); \
        }

    //! \addtogroup Base_PublicUtilities
    //! \{

    //! Exception fired if an argument is invalid
    DECLARE_EXCEPTION( InvalidArgumentException );

    //! Exception fired if an argument is out of range
    DECLARE_EXCEPTION( OutOfRangeException );

    //! Exception fired if a property access fails
    DECLARE_EXCEPTION( PropertyException );

    //! Runtime exception
    DECLARE_EXCEPTION( RuntimeException );

    //! Exception to be thrown to indicate logical errors in program flow
    DECLARE_EXCEPTION( LogicalErrorException );

    //! Exception to be thrown to indicate an access error
    DECLARE_EXCEPTION( AccessException );

    //! Exception to be thrown to indicate an access error
    DECLARE_EXCEPTION( TimeoutException );

    //! Exception to be thrown to indicate the the result of a dynamic cast was zero
    DECLARE_EXCEPTION( DynamicCastException );

    //! \}

    /*-----------------------------------------------------------------*/
    // Utilities
    /*-----------------------------------------------------------------*/

        /**
        \brief printf like creation of exceptions
        \ingroup Base_PublicImpl
        */
        template <class E>
        class ExceptionReporter
        { 
        public:
            ExceptionReporter( const char* sourceFileName, unsigned int sourceLine )
                    : m_SourceFileName( sourceFileName )
                    , m_SourceLine( sourceLine )
            { }

            E
            Report( const char* pFormat, ... )
            {
                char pBuffer[ 256 ];
                va_list vap;
                va_start( vap, pFormat );

#if defined (_MSC_VER)
#   if (_MSC_VER >= 1400) // new VS8 secure crt
                vsnprintf_s( pBuffer, sizeof pBuffer, _TRUNCATE, pFormat, vap );
#   else // defined (_MSC_VER >= 1400)
                _vsnprintf( pBuffer, sizeof pBuffer, pFormat, vap );
#   endif // defined (_MSC_VER >= 1400)
#else //defined (_MSC_VER) 
                // others as gcc
                vsnprintf( pBuffer, sizeof pBuffer, pFormat, vap );
#endif // defined (_MSC_VER) 

                return E( pBuffer, m_SourceFileName.c_str(), m_SourceLine );
            };

            E
            Report( const std::string &s )
            {
              return E( s.c_str(), m_SourceFileName.c_str(), m_SourceLine );
            }

            E
            Report( const std::stringstream& str)
            {
                return E(str.str().c_str(), m_SourceFileName.c_str(), m_SourceLine);
            }


        private:
            gcstring m_SourceFileName;
            unsigned int m_SourceLine;

        };

    //! \addtogroup Base_PublicImpl
    //! \{

    //! Fires an exception, e.g. throw EXCEPTION("%ld too large", Value);
    #define GENERIC_EXCEPTION GenICam::ExceptionReporter<GenICam::GenericException>(__FILE__, __LINE__).Report

    //! Fires an invalid argument exception, e.g. throw INVALID_ARGUMENT("%ld too large", Value);
    #define INVALID_ARGUMENT_EXCEPTION GenICam::ExceptionReporter<GenICam::InvalidArgumentException>(__FILE__, __LINE__).Report

    //! Fires an out of range exception, e.g. throw OUT_OF_RANGE_EXCEPTION("%ld too large", Value);
    #define OUT_OF_RANGE_EXCEPTION GenICam::ExceptionReporter<GenICam::OutOfRangeException>(__FILE__, __LINE__).Report

    //! Fires an property exception, e.g. throw PROPERTY_EXCEPTION("%ld too large", Value);
    #define PROPERTY_EXCEPTION GenICam::ExceptionReporter<GenICam::PropertyException>(__FILE__, __LINE__).Report

    //! Fires a runtime exception, e.g. throw RUNTIME_EXCEPTION("buh!")
    #define RUNTIME_EXCEPTION GenICam::ExceptionReporter<GenICam::RuntimeException>(__FILE__, __LINE__).Report

    //! Fires a logical error exception, e.g. throw LOGICAL_ERROR_EXCEPTION("Should never reach this point")
    #define LOGICAL_ERROR_EXCEPTION GenICam::ExceptionReporter<GenICam::LogicalErrorException>(__FILE__, __LINE__).Report

    //! Fires a access error exception, e.g. throw ACCESS_ERROR_EXCEPTION("Not ecverybody")
    #define ACCESS_EXCEPTION GenICam::ExceptionReporter<GenICam::AccessException>(__FILE__, __LINE__).Report

    //! Fires a timeout error exception, e.g. throw TIMEOUT_EXCEPTION("Not ecverybody")
    #define TIMEOUT_EXCEPTION GenICam::ExceptionReporter<GenICam::AccessException>(__FILE__, __LINE__).Report

    //! Fires a dynamic cast exception, e.g. throw DYNAMICCAST_EXCEPTION("Not ecverybody")
    #define DYNAMICCAST_EXCEPTION GenICam::ExceptionReporter<GenICam::DynamicCastException>(__FILE__, __LINE__).Report
        
#ifdef _MSC_VER
    //! Range check for int64
    #define CHECK_RANGE_I64(_Value, _Min, _Max, _Inc) \
        if((int64_t)(_Value) < (int64_t)(_Min)) \
            throw OUT_OF_RANGE_EXCEPTION("Value = %I64d must be equal or greater than Min = %I64d", (int64_t)(_Value), (int64_t)(_Min)); \
        else if((int64_t)(_Value) > (int64_t)(_Max)) \
            throw OUT_OF_RANGE_EXCEPTION("Value = %I64d must be equal or smaller than Max = %I64d", (int64_t)(_Value), (int64_t)(_Max)); \
        else if( (int64_t)(_Inc)!= 0 && ((int64_t)(_Value) - (int64_t)(_Min)) % (int64_t)(_Inc) != 0) \
            throw OUT_OF_RANGE_EXCEPTION("The difference between Value = %I64d and Min = %I64d must be dividable without rest by Inc = %I64d", (int64_t)(_Value), (int64_t)(_Min), (int64_t)(_Inc));
#else
    //! Range check for int64
    #define CHECK_RANGE_I64(_Value, _Min, _Max, _Inc) \
        if((int64_t)(_Value) < (int64_t)(_Min)) \
            throw OUT_OF_RANGE_EXCEPTION("Value = %Ld must be equal or greater than Min = %Ld", (int64_t)(_Value), (int64_t)(_Min)); \
        else if((int64_t)(_Value) > (int64_t)(_Max)) \
            throw OUT_OF_RANGE_EXCEPTION("Value = %Ld must be equal or smaller than Max = %Ld", (int64_t)(_Value), (int64_t)(_Max)); \
        else if( ((int64_t)(_Value) - (int64_t)(_Min)) % (int64_t)(_Inc) != 0) \
            throw OUT_OF_RANGE_EXCEPTION("The difference between Value = %Ld and Min = %Ld must be dividable without rest by Inc = %Ld", (int64_t)(_Value), (int64_t)(_Min), (int64_t)(_Inc));
#endif

    //! Range check for float
    #define CHECK_RANGE_FLT(_Value, _Min, _Max) \
      if ((_Value) < (_Min)) \
        throw OUT_OF_RANGE_EXCEPTION( "Value %f must be greater than or equal %f", (_Value), (_Min) ); \
      else if ((_Value) > (_Max)) \
        throw OUT_OF_RANGE_EXCEPTION( "Value %f must be smaller than or equal %f", (_Value), (_Max) );

    //! Checks if a dynamic_cast is possible
    #define CHECK_DYNAMIC_CAST_POINTER( _Pointer )\
      assert( (_Pointer) != NULL ); \
      if (NULL == (_Pointer)) throw LOGICAL_ERROR_EXCEPTION( "Unexpected type in dynamic cast" )

    //! \}

}

#pragma pack(pop)

#endif // GENCAM_EXCEPTION_H_
