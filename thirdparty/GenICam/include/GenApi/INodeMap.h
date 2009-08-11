//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/INodeMap.h,v 1.13 2007/08/16 13:56:40 hartmut_nebelung Exp $
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
\brief  Definition of interface INodeMap  
\ingroup GenApi_PublicInterface
*/

#ifndef GENAPI_INODEMAP_H
#define GENAPI_INODEMAP_H

#include <Base/GCBase.h>
#include "INode.h"
#include "IPort.h"

#pragma warning ( push )
#pragma warning ( disable : 4251 ) // XXX needs to have dll-interface to be used by clients of class YYY

namespace GenApi
{

    //*************************************************************
    // INodeMap interface
    //*************************************************************

    /**
    \brief Interface to access the node map 
    \ingroup GenApi_PublicInterface
    */
    interface   INodeMap 
    {
        //! Retrieves all nodes in the node map
        virtual void GetNodes(NodeList_t &Nodes) const =  0;

        //! Retrieves the node from the central map by Name
        virtual INode* GetNode( const GenICam::gcstring& Name) const = 0;

        //! Invalidates all nodes
        virtual void InvalidateNodes() const =  0;

        //! Connects a port to a port node with given name
        virtual bool Connect( IPort* pPort, const GenICam::gcstring& PortName) const = 0;

        //! Connects a port to the standard port "Device"
        virtual bool Connect( IPort* pPort) const = 0;

        //! Get device name 
        /*! The device name identifies a device instance, e.g. for debuggin purposes.
        The default ist "Device". */
        virtual GenICam::gcstring GetDeviceName() = 0;

        //! Fires nodes which have a polling time
        virtual void Poll( int64_t ElapsedTime ) = 0;
    };
}

#pragma warning ( pop )

#endif // ifndef GENAPI_INODEMAP_H
