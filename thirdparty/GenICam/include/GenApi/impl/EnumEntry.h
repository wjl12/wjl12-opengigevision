//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/EnumEntry.h,v 1.12 2007/08/16 13:56:24 hartmut_nebelung Exp $
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
\brief    Definition of CEnumEntry
*/

#ifndef GENAPI_ENUMENTRY_H
#define GENAPI_ENUMENTRY_H

#include "../IEnumEntry.h"
#include "Node.h"
#include "NodeT.h"
#include "BaseT.h"
#include "ValueT.h"
#include "EnumEntry.h"

#pragma warning ( push )
#pragma warning ( disable : 4275 ) // non dll-interface XXX used as base for  dll-interface class YYY

namespace GenApi
{

    //! Mapping of Enum Values to symbolic values
    //*************************************************************
    // EnumEntry class
    //*************************************************************

    class GENAPI_DECL CEnumEntryImpl: public CNodeImpl, public IEnumEntry
    {
    public:
        //! DefaultContructor
        CEnumEntryImpl()
            :m_Value(0){};

        //-------------------------------------------------------------
        // INode implementation
        //-------------------------------------------------------------
        GenICam::gcstring InternalGetDisplayName() const
        {
            if (m_DisplayName != "")
                return m_DisplayName;
            else
                return  m_Symbolic;
        }


        //-------------------------------------------------------------
        // IValue implementation
        //-------------------------------------------------------------
        //! Get value of the node as string
        virtual GenICam::gcstring InternalToString(bool /*Verify = false*/)
        {
            const Lock l( GetLock() );
            GenICam::gcstring valueString;
            Value2String(m_Value, valueString);
            return valueString;
        }
        //! Set content of the node as string
        #pragma BullseyeCoverage off
        virtual void InternalFromString(const GenICam::gcstring& ValueStr, bool /*Verify*/)
        {
            assert(false);
            throw LOGICAL_ERROR_EXCEPTION("EnumEntry %s can't set value from string  : %s. Use derived class!", m_Name.c_str(), ValueStr.c_str() );
        }
        #pragma BullseyeCoverage on

        //-------------------------------------------------------------
        // IEnum implementation
        //-------------------------------------------------------------

        //! Get symbolic enum value
        virtual GenICam::gcstring GetSymbolic() const
        {
            const Lock l( GetLock() );
            return m_Symbolic;
        }

        virtual int64_t GetValue()
        {
            return m_Value;
        }


        //! Convert map
        BEGIN_CONVERT_MAP
            CONVERT_ENTRY(Value, m_Value)
            CONVERT_STRING_ENTRY(Symbolic, m_Symbolic)
            CHAIN_CONVERT_MAP(CNodeImpl)
        END_CONVERT_MAP

    private:
        int64_t m_Value;
        GenICam::gcstring m_Symbolic;

    };

    class GENAPI_DECL CEnumEntry : public BaseT< ValueT< NodeT < CEnumEntryImpl > > >
    {
    };
}

#pragma warning ( pop )

#endif // ifndef GENAPI_ENUMENTRY_H
