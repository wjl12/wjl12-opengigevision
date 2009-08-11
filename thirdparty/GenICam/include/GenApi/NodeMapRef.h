//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/NodeMapRef.h,v 1.11.2.1 2009/02/05 11:29:32 hartmut_nebelung Exp $
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
\brief    Definition of CNodeMapRef
\ingroup GenApi_PublicInterface
*/

#ifndef GENAPI_NODEMAPREF_H
#define GENAPI_NODEMAPREF_H

#include "Pointer.h"
#include "DLLLoad.h"

namespace GenApi
{
    /**
    \brief Smartpointer for NodeMaps with create function
    \ingroup GenApi_PublicInterface
    */
    class GENAPI_DECL CNodeMapRef 
    {
    public:
        //! Constructor
        CNodeMapRef(GenICam::gcstring DeviceName = "Device" );

        //! Destructor
        virtual ~CNodeMapRef();

        //! Creates the object from a DLL whose name is deduced from vendor anf model name
        void _LoadDLL(GenICam::gcstring VendorName, GenICam::gcstring ModelName);

        //! Creates the object from a DLL with given file name
        void _LoadDLL(GenICam::gcstring FileName);

        //! Creates the object from a XML file whose name is deduced from vendor anf model name
        void _LoadXMLFromFile(GenICam::gcstring VendorName, GenICam::gcstring ModelName);

        //! Creates the object from a XML file with given file name
        void _LoadXMLFromFile(GenICam::gcstring FileName);

        //! Creates the object from XML data given in a string
        void _LoadXMLFromString(const GenICam::gcstring& XMLData);

        //! Get device name 
        virtual GenICam::gcstring _GetDeviceName();

        //! Fires nodes which have a polling time
        virtual void _Poll( int64_t ElapsedTime );

        //! Destroys the node map
        void _Destroy();

        //! Clears the cache of the camera description files
        static bool _ClearXMLCache();

        //----------------------------------------------------------------
        // INodeMap
        //----------------------------------------------------------------

        //! Retrieves all nodes in the node map
        virtual void _GetNodes(NodeList_t &Nodes) const;


        //! Retrieves the node from the central map by name
        virtual INode* _GetNode( const GenICam::gcstring& key) const;

        //! Invalidates all nodes
        virtual void _InvalidateNodes() const;

        //! Connects a port to a port node with given name
        virtual bool _Connect( IPort* pPort, const GenICam::gcstring& PortName) const;

        //! Connects a port to the standard port "Device"
        virtual bool _Connect( IPort* pPort) const;

        //! Pointer to the NodeMap
        #pragma warning ( push )
        #pragma warning ( disable : 4251 ) // XXX needs to have dll-interface to be used by clients of class YYY
        CPointer<INodeMap, INodeMap> _Ptr;
        #pragma warning ( pop )


        //! Retrieve all top-level nodes in the node map
    protected:
        //----------------------------------------------------------------
        // Implementation
        //----------------------------------------------------------------

        //! Creates the DLL filename
        GenICam::gcstring _MakeDLLFileName(GenICam::gcstring VendorName, GenICam::gcstring ModelName);

        //! Creates the object from a DLL with given file name without initializing
        void _InternalLoadDLL(GenICam::gcstring FileName);

        //! Initializes the references
        virtual void _Initialize()
        {}
        
        //! Loads the xmlloader dll
        void _GetXmlLoader();
    private:
        //! False, when the dll already has been loaded
        bool _loaded;

        //! The name of this device
        GenICam::gcstring _DeviceName;

        CNodeMapRef(const CNodeMapRef&);
        CNodeMapRef& operator=(const CNodeMapRef&);

    };
}

#endif // ifndef GENAPI_NODEMAPPTR_H
