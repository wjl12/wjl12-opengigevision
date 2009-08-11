//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Hartmut Nebelung
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/Command.h,v 1.10.2.1 2008/07/21 18:25:16 hartmut_nebelung Exp $
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
\brief    Definition of CCommand
*/

#ifndef GENAPI_COMMAND_H
#define GENAPI_COMMAND_H

#if defined (_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#include "../ICommand.h"
#include "Node.h"
#include "BaseT.h"
#include "ValueT.h"
#include "NodeT.h"
#include "CommandT.h"

#pragma warning ( push )
#pragma warning ( disable : 4275 ) // non dll-interface XXX used as base for  dll-interface class YYY

namespace GenApi
{

    //*************************************************************
    // CCommand class
    //*************************************************************

    /*! 
    * \brief ICommand implementation
    *
    * May be used as a toggle, that resets itself
    * Supposed to work together with a IInteger node, e.g. CMaskedInt,
    * for setting the concrete values.
    *
    * \ingroup GenApi_PublicImpl
    */

    class GENAPI_DECL CCommandImpl : public  ICommand, public CNodeImpl
    {
    public:
        //! Constructor
        CCommandImpl();

        //-------------------------------------------------------------
        //! \name IBase implementation
        EAccessMode InternalGetAccessMode() const;

        //-------------------------------------------------------------
        //! \name INode implementation

        //! Invalidates the node if the polling time has elapsed
        virtual void Poll( int64_t ElapsedTime );

        //-------------------------------------------------------------
        //! \name IValue implementation
        //\{
        // Get value of the node as string
        virtual GenICam::gcstring InternalToString(bool Verify = false);

        // Set value of the node as string
        virtual void InternalFromString(const GenICam::gcstring& valueString, bool Verify = true);

        // \}

    protected:
        //-------------------------------------------------------------
        //! \name ICommand implementation
        //\{
        // do it!
        virtual void InternalExecute();

        // done ?
        virtual bool InternalIsDone() const;
        // \}


    public:
        //! \name INodePrivate implementation
        //\{
        virtual bool IsTerminalNode() const { return m_pValue == NULL; }
        virtual void FinalConstruct();

        //\}

        //-------------------------------------------------------------
        // Initializing
        //-------------------------------------------------------------
        BEGIN_CONVERT_MAP
            CONVERT_NODE_REFERENCE(pValue, m_pValue, IInteger)
            CONVERT_ENTRY(Value, m_Value)
            CONVERT_ENTRY(CommandValue, m_CommandValue)
            CONVERT_NODE_REFERENCE(pCommandValue, m_pCommandValue, IInteger)
            CHAIN_CONVERT_MAP(CNodeImpl)
        END_CONVERT_MAP

    protected:

        //-------------------------------------------------------------
        // Member variables
        //-------------------------------------------------------------

        //! placeholder if no register node used
        mutable bool m_Value;
        mutable bool m_PendingCallback;
        //! Value for On
        int64_t m_CommandValue;

        //! Pointer to the the source of the command value
        IInteger *m_pCommandValue;

        //! Pointer to the associated register
        IInteger *m_pValue;
    };


    class GENAPI_DECL CCommand : public BaseT< ValueT< NodeT< CommandT< CCommandImpl > > > >
    {
    };
}
#pragma warning ( pop )

#endif // GENAPI_COMMAND_H
