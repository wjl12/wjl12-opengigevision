//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  AH
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/DllManager.h,v 1.9 2007/08/16 13:56:23 hartmut_nebelung Exp $
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
\brief    Definition of CDllManager
*/

#ifndef GENAPI_DLLMANAGER_H
#define GENAPI_DLLMANAGER_H
#if MSC_VER>1000
#pragma once
#endif // MSC_VER>1000

namespace GenICam
{
    class gcstring;
}

#include <map>
#include "../Synch.h"
#include <Base/GCBase.h>


namespace GenApi
{
    /** 
      \brief Platform-independent dynamic library loader

      The DllManager allows to load dynamic libraries and to search for symbols within loaded 
      libraries. A book-keeping is performed. All libraries which are not unloaded explicitly, 
      are automatically unloaded when the DllManager is deleted. 

      It is not possible to instantiate a DllManager object, instead the DllManager class exposes
      static member functions.
    */
    class GENAPI_DECL DllManager : public LockableObject<DllManager>
    {
    public:

        typedef void * Handle;
        static const Handle INVALID_HANDLE;

        typedef void * Symbol;
        static const Symbol INVALID_SYMBOL;

        //! Load a dynamic library
        static Handle LoadLib(const GenICam::gcstring& fileName);

        //! Unload a dynamic library
        static bool FreeLib(const Handle handle);

        //! Retrieve a symbol from a dynamic library
        static Symbol FindSymbol(const Handle handle, const GenICam::gcstring& symbolName ); 

        ~DllManager(void);

    private:
        //! Platform specific code to load a library
        static Handle InternalLoadLib(const GenICam::gcstring& fileName);

        //! Platform specific code to find a symbol in a library
        static Symbol InternalFindSymbol(const Handle handle, const GenICam::gcstring& symbolName);

        //! Platform specfic code to unload a library
        static bool InternalFreeLib(const Handle handle);

        //! Returns the one and only DllManager(singleton pattern)
        static DllManager& GetInstance(void);


        //! @name Hide constructors and assignment operator
        //@{
        //! (singleton cannot be constructed outside)
        DllManager(void) {}
        DllManager(const DllManager&);
        DllManager& operator=(const DllManager&);
        //@}


        //! Container used to store handles
        //typedef std::multiset<Handle>  DllTable;
        typedef std::map<GenICam::gcstring, Handle>  DllTable;

        //! Container storing the library handles
        DllTable m_DllTable;

    };


}
#endif // DLLMANAGER_H
