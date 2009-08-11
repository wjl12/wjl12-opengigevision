//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/VariableDelegate.h,v 1.12 2007/11/13 19:41:06 hartmut_nebelung Exp $
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
\brief    Definition of CVariableDelegate
*/

#ifndef GENAPI_VARIABLEDELEGATE_H
#define GENAPI_VARIABLEDELEGATE_H

#include "../GenApiDll.h"
#include "Node.h"

#include <map>

#ifdef _MSC_VER // *JS*
#pragma warning(push)
#pragma warning(disable: 4251) // class 'xxx' needs to have dll-interface to be used by clients of class 'yyy'
#endif

namespace GenApi
{

    class CNodeMap;

    // 
    //*************************************************************
    // VariableDelegate class
    //*************************************************************
    /**
    * \ingroup internal_impl
    *
    * \brief Mapping of variable names to values
    */
    class GENAPI_DECL CVariableDelegate
    {
    public:
        //! mapping of  variable names to values
        typedef std::map< GenICam::gcstring, INode* > Name2pValue_t;

        //! Default Constructor
        CVariableDelegate();

        //! Default Destructor
        virtual ~CVariableDelegate() {}

        //-------------------------------------------------------------
        // IValue implementation
        //-------------------------------------------------------------

        //! Get value of all variables as cobined string
        virtual GenICam::gcstring ToString();

        //-------------------------------------------------------------
        // IBase implementation
        //-------------------------------------------------------------
        //!    Get    the    access mode    of the node
        virtual EAccessMode GetAccessMode() const;

        //! Gets the Value of the Varaible with the given Name
        virtual bool GetValue(const GenICam::gcstring &nameStr, int64_t * value, bool Verify = false);

        //! Gets the Value of the Varaible with the given Name
        virtual bool GetValue(const GenICam::gcstring &nameStr, double * value, bool Verify = false);

        //! Inserts a variable in the variable map
        virtual bool AddEntry(const GenICam::gcstring &Value, const GenICam::gcstring &Symbolic, const GenICam::gcstring &parentName, INodeMapPrivate * pNodeMap);

        //! Gets Variables and attributes added to the map
        virtual void GetVariablesAndAttributes(GenICam::gcstring &Variables, GenICam::gcstring &Attributes);

    public:
        //! Map holding pointers to Values keyed with the variable name
        Name2pValue_t m_Map;

    };
};

#ifdef _MSC_VER // *JS*
#pragma warning(pop)
#endif

#endif // ifndef GENAPI_VARIABLEDELEGATE_H
