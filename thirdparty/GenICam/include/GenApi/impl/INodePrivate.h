//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  MargretAlbrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/INodePrivate.h,v 1.8 2007/12/18 17:10:15 hartmut_nebelung Exp $
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
\brief    Definition of interface INodePrivate
*/


#ifndef GENAPI_INODEPRIVATE_H
#define GENAPI_INODEPRIVATE_H

#include "../GenApiDll.h"
#include "../Types.h"
#include "../INode.h"
#include "../INodeMap.h"

#include <vector>
#include <set>

namespace GenApi
{
    //*************************************************************
    // INodePrivate interface
    //*************************************************************

    interface INodePrivate;
    interface INodeMapPrivate;

    //! a list of node references using the INodePrivate interface
    typedef std::vector<INodePrivate*> NodePrivateList_t;
    //! Set of pointers to nodes
    typedef std::set< INodePrivate* > NodeSet_t;


    /**
    \brief Interface including the methods for node construction common to all nodes
    \ingroup GenApi_PublicImpl
    */
    interface GENAPI_DECL INodePrivate : virtual public INode
    {
        //! Three different modes of operation for INodePrivate::SetInvalid()
        enum ESetInvalidMode
        {
            simOnlyMe,  //!< Invalidate only the node itself
            simAll  //!< Invalidate the node and all of its dependents
        };

        //! Registers the node in the node map
        virtual void Register(GenApi::INodeMapPrivate* const pNodeMap, const char *pNodeType, const char *pName, const char *pNameSpace) = 0;

        //! Set property by name as string
        /*! return value true if the property was handled; false else */
        virtual bool SetProperty(const GenICam::gcstring& pPropertyName, const GenICam::gcstring& pValueStr) = 0;
        
        //! Set property by name as string with attribute
        /*! return value true if the property was handled; false else */
        virtual bool SetProperty(const GenICam::gcstring& pPropertyName, const GenICam::gcstring& pValueStr, const GenICam::gcstring& pAttributeStr) = 0;

        //! Create the set of dependent nodes
        virtual bool PropagateDependency( GenApi::NodePrivateList_t &Dependents ) = 0;

        //! Create the set of terminal nodes
        virtual void PropagateTerminals( GenApi::NodeSet_t &Terminals, NodePrivateList_t &Visited ) = 0;

        //! Initializes the object
        virtual void FinalConstruct() = 0;

        //! Update the registered callbacks
        virtual void FireCallBacks(bool allDependents = false) = 0;    

        //! Invalidate the node resp. the node and all of its dependents
        virtual void SetInvalid(ESetInvalidMode simMode) = 0;

        //! get the dependent nodes
        virtual void GetDependencies( GenApi::NodeList_t &Dependents ) const = 0;

        //! Returns the set of parents
        virtual void GetParents(GenApi::NodeList_t &) const = 0;

        //! Returns ture, if the node is a terminal node, i.e., a node which has to trigger the on-change callbacks
        virtual bool IsTerminalNode() const = 0;

        //! Returns the list of terminal nodes
        virtual void GetTerminalNodes(NodeList_t &) const = 0;

        //! Returns true, if the node is a top-level node, i.e, a node having no parents
        virtual bool IsTopLevelNode() const = 0;

        //! Invalidates the node if the polling time has elapsed
        virtual void Poll( int64_t ElapsedTime ) = 0;

        //! Enforces a virtual destructor on every node class
        virtual ~INodePrivate()
        {
        }

    };


}

#endif // ifndef GENAPI_INODEPRIVATE_H
