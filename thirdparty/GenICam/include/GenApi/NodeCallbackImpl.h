//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Hartmut Nebelung
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/NodeCallbackImpl.h,v 1.14.2.1 2008/07/14 08:30:46 hartmut_nebelung Exp $
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
\brief    Implementation helpers for NodeCallback
\ingroup GenApi_PublicImpl

Defines a bunch of templates for creating callback-functors taking a INode pointer
and returning void.

\note does not work with all stl containers, i.e. std::map

*/

#ifndef GENAPI_NODECALLBACKIMPL_H
#define GENAPI_NODECALLBACKIMPL_H

#include "NodeCallback.h"

namespace GenApi
{
    using GenApi::NodeCallback;
    using GenApi::Base_Callback1Body;

    /**
    \brief callback body instance for INode pointers
    \ingroup GenApi_PublicImpl
    */
    // xxx class GENAPI_DECL NodeCallbackBase : public Base_Callback1Body<INode*>
    class NodeCallbackBase : public Base_Callback1Body<INode*>
    {
    };

    /***************************************************************************/
    // C Functions as callbacks
    /***************************************************************************/

    /**
    \brief Container for a function pointer 
    \ingroup GenApi_PublicImpl
    */
    template <class Function>
    class Function_NodeCallbackBody : public NodeCallbackBase
    {
    public:
        //! Constructor
        Function_NodeCallbackBody( const Function& function ) : 
            m_pFunction( function ) 
        {}
    
        //! execute operation: call the function
        void operator()(INode* pNode) const 
        { 
            if (m_pFunction) 
                m_pFunction( pNode );
        }

        //! virtual copy constructor
        Function_NodeCallbackBody<Function>* clone() const
        { 
            return new Function_NodeCallbackBody<Function>(*this); 
        }
    private:
        //! the callback function
        const Function m_pFunction;

        //! no assignemt operator
        Function_NodeCallbackBody& operator=(Function_NodeCallbackBody&);
    };

    /*-----------------------------------------------------------------------------*/

    /**
    \brief make a new callback object for C functions
    \ingroup GenApi_PublicImpl
    */
    template <class Function>
    NodeCallback make_NodeCallback( Function function )
    {
        return NodeCallback(new Function_NodeCallbackBody<Function>(function));
    }

    /*-----------------------------------------------------------------------------*/

    /**
    \brief Register a C-function as a callback
    \ingroup GenApi_PublicUtilities
    */
    template<class Function>
    intptr_t Register( INode* pNode, Function f)
    {
        NodeCallback cb(make_NodeCallback(f));
        return pNode->RegisterCallback(cb);
    }

    /***************************************************************************/
    // C++ Member functions as callbacks
    /***************************************************************************/

    /**
    \brief Container for a member function pointer 
    \ingroup GenApi_PublicImpl
    */
    template <class Client, class Member>
    class Member_NodeCallbackBody : public NodeCallbackBase
    {
    public:
        //! Member function type
        typedef void (Client::*PMEMBERFUNC)(INode*);

        //! Constructor
        Member_NodeCallbackBody( Client& client, Member member ) : 
            m_Client(client), 
            m_pMemberFunc(member) 
        {}

        //! execute operation
        void operator()( INode* pNode ) const 
        { 
            if (m_pMemberFunc)
                (m_Client.*m_pMemberFunc)( pNode ); 
        }
        
        //! virtual copy constructor
        Member_NodeCallbackBody<Client,Member>* clone() const
        {
            return new Member_NodeCallbackBody<Client,Member>( m_Client, m_pMemberFunc );
        }

    private:
        //! The object the method function belongs to
        Client& m_Client;

        //! The method to call
        PMEMBERFUNC m_pMemberFunc;

        //! no assognment operator
        Member_NodeCallbackBody& operator=(Member_NodeCallbackBody&);
    };

    /*-----------------------------------------------------------------------------*/

    /**
    \brief make a new callback object for member functions
    \ingroup GenApi_PublicImpl
    */
    template <class Client, class Member>
    NodeCallback make_NodeCallback( Client& client, Member member )
    {
        return NodeCallback(new Member_NodeCallbackBody<Client,Member>(client, member));
    }

    /*-----------------------------------------------------------------------------*/

    /**
    \brief Register a C++-member function a callback
    \ingroup GenApi_PublicUtilities
    */
    template<class Client, class Member>
    intptr_t Register( INode* pNode, Client &c, Member m ) 
    {
        NodeCallback cb(make_NodeCallback(c, m));
        return pNode->RegisterCallback(cb);
    }

}

#endif // GENAPI_NODECALLBACKIMPL_H
