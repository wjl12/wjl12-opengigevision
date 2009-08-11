//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Hartmut Nebelung
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/Synch.h,v 1.10 2007/08/16 13:56:42 hartmut_nebelung Exp $
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
\brief    Definition of Lock classes
\ingroup GenApi_PublicImpl
*/

#ifndef GENAPI_SYNCH_H
#define GENAPI_SYNCH_H

#include "GenApiDll.h"

#if defined (_WIN32) && (_WIN32_WINNT < 0x0400)
#error ("Value of _WIN32_WINNT must be at least 0x0400")
#endif

#if defined (_WIN32)
#   include <windows.h>
#elif defined (__unix__)
#   include <pthread.h>
#   include <errno.h>
#else
#   error No/unknown platform thread support
#endif


#ifndef THREADING_MODEL
/**
* \brief Controls the threading model used for the GenApi. 
* 
* The default threading model is the MultiThreadingModel, i.e. 
* using the classes from multiple threads is safe.
*
* When you are sure that there is no concurrent access of the GenApi 
* classes by multiple threads, 
* THREADING_MODEL can be defined as SingleThreadingModel. In this case
* the synchronization objects are dummy objects. 
*
* When defining THREADING_MODEL yourself, ensure that for *all* translation units
* (this includes the libarary code as well as your application code) are compiled
* with the same value for THREADING_MODEL. Otherwise you will encounter strange
* runtime errors.
*/
# define THREADING_MODEL MultiThreadingModel
#endif




namespace GenApi {


    //-----------------------------------------------------------------
    // FakeLock
    //-----------------------------------------------------------------
    /**
    \brief A lock that does nothing
    \ingroup GenApi_PublicImpl
    */
    class GENAPI_DECL FakeLock
    {
    public:
        bool TryLock() { return true; }
        void Lock()    { return; }
        void Unlock()  { return; }
    };

#if defined (_WIN32)
    /**
    \brief A lock class
    \ingroup GenApi_PublicImpl
    */
    class GENAPI_DECL CLock
    {
        CRITICAL_SECTION m_csObject;
        CLock( const CLock& );
        CLock& operator=( const CLock& );
    public:
        CLock() { ::InitializeCriticalSection( &m_csObject );  }
        ~CLock(){ ::DeleteCriticalSection( &m_csObject );      }

    bool TryLock()  { return ::TryEnterCriticalSection( &m_csObject )!= 0; }
        void Lock()     { ::EnterCriticalSection( &m_csObject ); }
        void Unlock()   { ::LeaveCriticalSection( &m_csObject ); }
    };
#elif defined (__unix__)
    /**
    \brief A lock class
    \ingroup GenApi_PublicImpl
    */
    class CLock
    {
        pthread_mutex_t m_mtxObject;
        CLock( const CLock& );
        CLock& operator=( const CLock& );
    public:
        CLock() {
            pthread_mutexattr_t attr;
            pthread_mutexattr_init( &attr );
            pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
            pthread_mutex_init( &m_mtxObject, &attr );
            pthread_mutexattr_destroy( &attr );
        }
        ~CLock(){
            pthread_mutex_destroy( &m_mtxObject );
        }

        bool TryLock() {
            const int err = pthread_mutex_trylock( &m_mtxObject );
            if (err == 0)
                return true;
            if (err == EBUSY)
                return false;
            else
                throw RUNTIME_EXCEPTION( "CLock::TryLock() failed: %d (%s)", err, strerror( err ) );
        }
        void Lock() {
            const int err = pthread_mutex_lock( &m_mtxObject );
            if ( err != 0 )
                throw RUNTIME_EXCEPTION( "CLock::Lock() failed: %d (%s)", err, strerror( err ) );
        }
        void Unlock() {
            const int err = pthread_mutex_unlock( &m_mtxObject );
            if ( err != 0 )
                throw RUNTIME_EXCEPTION( "Mutex::unlock() failed: %d (%s)", err, strerror( err ) );
        }

    };
#else
#   error No/unknown platform thread support
#endif

    //-----------------------------------------------------------------
    // SingleThreadingModel
    //-----------------------------------------------------------------
    /**
    \brief policy class for single threading
    \ingroup GenApi_PublicImpl
    */
    class GENAPI_DECL SingleThreadingModel
    {
    public:
        /// The Lock Type
        typedef FakeLock LockType;
    };

    //-----------------------------------------------------------------
    // MultiThreadingModel
    //-----------------------------------------------------------------
    /**
    \brief policy class for multi threading
    \ingroup GenApi_PublicImpl
    */
    class GENAPI_DECL MultiThreadingModel
    {
    public:
        /// The Lock Type
        typedef CLock LockType;
    };

    //! The threading model used within the library
    //! \ingroup GenApi_PublicImpl
    typedef THREADING_MODEL ThreadingModel;

    class AutoLock
    {
        ThreadingModel::LockType& m_Lock;
    public:
        AutoLock(ThreadingModel::LockType& lock)
            : m_Lock(lock)
        {
            m_Lock.Lock();
        }

        ~AutoLock()
        {
            m_Lock.Unlock();
        }

    private:
        AutoLock& operator=(const AutoLock&);
        AutoLock(const AutoLock&);
    };

    //-----------------------------------------------------------------
    // template LockableObject<Object,ThreadingModel>
    //-----------------------------------------------------------------

    /**
    \brief Instance-Lock for an object
    \ingroup GenApi_PublicImpl
    */
    template< class Object, typename ThreadingModel=ThreadingModel >
    class GENAPI_DECL LockableObject {


    public:
        mutable typename ThreadingModel::LockType m_Lock;

        class Lock;
        friend class Lock;

        /*! A scopelevel Lock class.
        * Automatically aquires the lock when created and releases
        * it when destroyed.
        */
        class Lock {
            /// Reference to outer object
            const LockableObject<Object,ThreadingModel> &m_Object;
        public:
            Lock( const LockableObject<Object,ThreadingModel>& obj) : m_Object(obj) {
                m_Object.m_Lock.Lock();
            }

            ~Lock(){
                m_Object.m_Lock.Unlock();
            }
        private:
            Lock& operator=( const Lock& );
        };
        /// Get a new lock
        Lock GetLock() const {
            return Lock( *this );
        }
    };

} // namespace Genapi

#endif // GENAPI_SYNCH_H
