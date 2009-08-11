//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/EnumDelegate.h,v 1.10 2007/08/16 13:56:24 hartmut_nebelung Exp $
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
\brief    Definition of CEnumDelegate and EnumEntryList_t.
*/

#ifndef GENAPI_ENUMDELEGATE_H
#define GENAPI_ENUMDELEGATE_H

#include <Base/GCStringVector.h>
#include "Node.h"


namespace GenApi
{


    interface IEnumEntry;

    
    //! Mapping of Enum Values to symbolic values
    //*************************************************************
    // EnumDelegate class
    //*************************************************************
    class GENAPI_DECL CEnumDelegate
        : public LockableObject<CEnumDelegate, ThreadingModel>
    {


    //! mapping of enum values to enum entry node
        typedef NodeList_t EnumEntryList_t;

    public:
        //! DefaultConstructor
        CEnumDelegate();
        virtual ~CEnumDelegate();
        //-------------------------------------------------------------
        // IValue implementation
        //-------------------------------------------------------------
        //! Get value of the node as string
        virtual GenICam::gcstring ToString();


        /**
        * Maps a numerical value to it's corresponding symbolic representation. */
        virtual void ToString(GenICam::gcstring &ValueStr, const int64_t &value);


        /** maps a symbolic value to its corresponding numeric value  */
        virtual bool FromString(const GenICam::gcstring &symbolicStr, int64_t &value);
    
        /** Returns the enum entry that is representing the numerical value. 
            Returns NULL when there is no entry representing the numerical value. */
        IEnumEntry* IntValueToEntry( const int64_t& value);



        /** retrieves the list of  symbolic values
        * @return the list
        */
        virtual void GetSymbolics(GenICam::gcstring_vector& Symbolics);
         /** retrieves the list of enumeration entries
        * @return the list
        */
        virtual void GetEntries(NodeList_t & Entries);

       /** AddEntry inserts an entry into the map
        */
        virtual void AddEntry(INode * pEnumEntry);


    private:
        CEnumDelegate(const CEnumDelegate&);
        CEnumDelegate& operator=(const CEnumDelegate&);

        EnumEntryList_t m_EnumEntryList;

    };
};

#endif // ifndef GENAPI_ENUMDELEGATE_H
