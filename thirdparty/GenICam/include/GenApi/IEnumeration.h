//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/IEnumeration.h,v 1.15 2007/08/16 13:56:40 hartmut_nebelung Exp $
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
\brief    Definition of interface IEnumeration
\ingroup GenApi_PublicInterface
*/

#ifndef GENAPI_IENUMERATION_H
#define GENAPI_IENUMERATION_H

#include <Base/GCStringVector.h>
#include "GenApiDll.h"
#include "IEnumEntry.h"
#include "Types.h"
#include "IValue.h"

#pragma warning ( push )
#pragma warning ( disable : 4251 ) // XXX needs to have dll-interface to be used by clients of class YYY


namespace GenApi
{
    //*************************************************************
    // Enumeration interface 
    //*************************************************************

    /**
    \brief Interface for enumeration properties
    \ingroup GenApi_PublicInterface
    */
    interface   IEnumeration  : virtual public IValue
    {

        //! Get list of symbolic Values
        virtual void GetSymbolics(StringList_t & Symbolics) = 0;

        //! Get list of entry nodes
        virtual void GetEntries(NodeList_t & Entries) = 0;

        //! Set string node value
        virtual IEnumeration& operator=(const GenICam::gcstring& ValueStr) = 0;

        //! Set integer node value
        virtual void SetIntValue(int64_t Value, bool Verify = true) = 0;

        //! Get string node value
        virtual GenICam::gcstring operator*() = 0;

        //! Get integer node value
        virtual int64_t GetIntValue(bool Verify = false) = 0;

        //! Get an entry node by name
        virtual IEnumEntry *GetEntryByName(const GenICam::gcstring& EntryName) = 0;

    };

}

#pragma warning ( pop )

#endif // ifndef GENAPI_IENUMERATION_H
