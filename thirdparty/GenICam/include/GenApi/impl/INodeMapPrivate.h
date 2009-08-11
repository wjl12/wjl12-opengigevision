//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/INodeMapPrivate.h,v 1.7 2007/08/16 13:56:24 hartmut_nebelung Exp $
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
\brief  Definition of interface INodeMapPrivate
\ingroup GenApi_PublicImpl
*/

#ifndef GENAPI_INODEMAPPRIVATE_H
#define GENAPI_INODEMAPPRIVATE_H

#include <Base/GCBase.h>
#include "../Synch.h"
#include "../Counter.h"
#include "../INodeMap.h"
#include "INodePrivate.h"

#pragma warning ( push )
#pragma warning ( disable : 4251 ) // XXX needs to have dll-interface to be used by clients of class YYY

namespace GenApi
{
    /**
    \brief NodeMap functions used for initilaization
    \ingroup GenApi_PublicImpl
    */
    interface   INodeMapPrivate : virtual public INodeMap
    {
        //! Adds a node to the map
        virtual void AddNode( const GenICam::gcstring& key, GenApi::INodePrivate *node) = 0;

        //! Set property by name as string
        /*! return value true if the property was handled; false else */
        virtual bool SetProperty(const GenICam::gcstring& pPropertyName, const GenICam::gcstring& pValueStr) = 0;

        //! Set property by name as stringwith attribute
        /*! return value true if the property was handled; false else */
        virtual bool SetProperty(const GenICam::gcstring& pPropertyName, const GenICam::gcstring& pValueStr, const GenICam::gcstring& pAttributeStr) = 0;

        //! Returns the lock which guards the node map
        virtual ThreadingModel::LockType& GetLock() const = 0;

        //! Returns the object which counts the depth of SetValue() call-chains
        virtual Counter& GetBathometer() = 0; 

        //! Retrieve all top-level nodes in the node map
        virtual void GetTopLevelNodes(NodeList_t &Nodes) const = 0;

        //! finalizes construction of the node map
        virtual void FinalConstruct() = 0;

    };

}

#pragma warning ( pop )

#endif // ifndef GENAPI_INODEMAPPROVATE_H
