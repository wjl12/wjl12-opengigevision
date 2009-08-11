//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/NodeMap.h,v 1.15 2007/11/13 19:41:05 hartmut_nebelung Exp $
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
\brief    Definition of CNodeMap and the type CreateFunc_t
*/

#ifndef GENAPI_NODEMAP_H
#define GENAPI_NODEMAP_H

#include <GenApi/GenApiVersion.h>
#include <GenApi/GenApiDll.h>
#include <GenApi/impl/INodeMapPrivate.h>
#include <GenApi/impl/INodePrivate.h>
#include <GenApi/IDestroy.h>
#include <GenApi/IDeviceInfo.h>
#include <GenApi/Synch.h>
#include <GenApi/impl/NodeMacros.h>
#include <Base/GCString.h>
#include <GenApi/impl/Value2String.h>

#include <map>

#ifdef _MSC_VER // *JS*
#pragma warning(push)
#pragma warning(disable: 4251) // class 'xxx' needs to have dll-interface to be used by clients of class 'yyy'
#endif

namespace GenApi
{

    typedef  INodePrivate*(*CreateFunc_t)( const GenICam::gcstring& );

    class string2node_t;
    /**
    \brief Hold a map of all nodes
    */
    class GENAPI_DECL CNodeMap
        : virtual public INodeMapPrivate
        , public IDeviceInfo
        , public IDestroy
    {
    public:
        //! Constructor
        CNodeMap(GenICam::gcstring DeviceName = "Device");

        //! Destructor
        virtual ~CNodeMap();

        //! creates a nodemap object
        static INodeMap* Create(GenICam::gcstring DeviceName);

        //-------------------------------------------------------------
        // INodeMap interface
        //-------------------------------------------------------------

        //! Retrieves all nodes in the node map
        virtual void GetNodes(NodeList_t &Nodes) const;

        //! Retrieves the node from the central map by name
        virtual INode* GetNode( const GenICam::gcstring& Name) const;

        // Returns the top-level odes
        virtual void GetTopLevelNodes(NodeList_t &Nodes) const;

        //! Invalidates all nodes
        virtual void InvalidateNodes() const;

        //! Connects a port to a port node with given name
        virtual bool Connect( IPort* pPort, const GenICam::gcstring& PortName) const;

        //! Connects a port to the standard port "Device"
        virtual bool Connect( IPort* pPort) const;

        //! Get device name 
        virtual GenICam::gcstring GetDeviceName();

        //! Fires nodes which have a polling time
        virtual void Poll( int64_t ElapsedTime );

        //-------------------------------------------------------------
        // INodeMapPrivate interface
        //-------------------------------------------------------------

        //! Adds a node to the map
        virtual void AddNode( const GenICam::gcstring& key, GenApi::INodePrivate *node);

        //! returns the locl object
        virtual ThreadingModel::LockType& GetLock() const { return m_Lock; } 

        //!returns the call stack depth counter object
        virtual Counter& GetBathometer() { return m_Bathometer; }

        //! finalizes construction of the node map
        virtual void FinalConstruct();


        //! Convert map (contains SetProperty)
        BEGIN_CONVERT_MAP
            CONVERT_STRING_ENTRY(ModelName,m_ModelName)
            CONVERT_STRING_ENTRY(VendorName,m_VendorName)
            CONVERT_STRING_ENTRY(ToolTip,m_ToolTip)
            CONVERT_STRING_ENTRY(StandardNameSpace,m_StandardNameSpace)
            CONVERT_ENTRY(SchemaMajorVersion,m_SchemaVersion.Major)
            CONVERT_ENTRY(SchemaMinorVersion,m_SchemaVersion.Minor)
            CONVERT_ENTRY(SchemaSubMinorVersion,m_SchemaVersion.SubMinor)
            CONVERT_ENTRY(MajorVersion,m_DeviceVersion.Major)
            CONVERT_ENTRY(MinorVersion,m_DeviceVersion.Minor)
            CONVERT_ENTRY(SubMinorVersion,m_DeviceVersion.SubMinor)
            CONVERT_STRING_ENTRY(ProductGuid,m_ProductGuid)
            CONVERT_STRING_ENTRY(VersionGuid,m_VersionGuid)
        END_CONVERT_MAP

        //----------------------------------------------------------------
        // implements IDeviceInfo
        //----------------------------------------------------------------

        //! Get the model name 
        virtual GenICam::gcstring GetModelName()
        {
            return m_ModelName;
        }

        //! Get the vendor name 
        virtual GenICam::gcstring GetVendorName()
        {
            return m_VendorName;
        }

        //! Get tool tip
        virtual GenICam::gcstring GetToolTip()
        {
            return m_ToolTip;
        }

        //! Get the standard name space which is used for retrieveing names without qualifier
        virtual GenICam::gcstring GetStandardNameSpace()
        {
            return m_StandardNameSpace;
        }

        //! Get the version of the DLL's GenApi implementation
        virtual void GetGenApiVersion(Version_t &Version, uint16_t &Build)
        {
            Version.Major = GENAPI_VERSION_MAJOR;
            Version.Minor = GENAPI_VERSION_MINOR;
            Version.SubMinor = GENAPI_VERSION_SUBMINOR;
            Build = GENAPI_VERSION_BUILD;
        }

        //! Get the schema version number 
        virtual void GetSchemaVersion(Version_t &Version)
        {
            Version = m_SchemaVersion;
        }

        //! Get the version of the device description file
        virtual void GetDeviceVersion(Version_t &Version)
        {
            Version = m_DeviceVersion;
        }
    
        //! Get the Guid describing the product
        virtual GenICam::gcstring GetProductGuid()
        {
            return m_ProductGuid;
        }

        //! Get the Guid describing the product version
        virtual GenICam::gcstring GetVersionGuid()
        {
            return m_VersionGuid;
        }

        //----------------------------------------------------------------
        // DeviceInfo
        //----------------------------------------------------------------

        //! model name 
        GenICam::gcstring m_ModelName;

        //! Get the vendor name 
        GenICam::gcstring m_VendorName;

        //! tool tip
        GenICam::gcstring m_ToolTip;

        //! standard name space
        GenICam::gcstring m_StandardNameSpace;

        //! schema version number 
        Version_t m_SchemaVersion;

        //! device description file's version number 
        Version_t m_DeviceVersion;

        //! Guid describing the product
        GenICam::gcstring m_ProductGuid;
        
        //! Guid describing the product version
        GenICam::gcstring m_VersionGuid;

        //! A list with all properties set
        GenICam::gcstring_vector m_PropertyNames;

        //-------------------------------------------------------------
        // Implementation
        //-------------------------------------------------------------

        //! Makes the object to destro itself
        virtual void Destroy();

        /**
        \brief Register class
        */
        static bool Register( GenICam::gcstring clasname, CreateFunc_t );

        //! Get the number of nodes in the map
        uint64_t GetNumNodes() const;

    protected:
        //! operator updating the dependencies because of the selector feature
        struct UpdateSelector;

        //! operator creating the selecting lists
        struct UpdateSelecting;

        //! propegate dependencies through the node map
        void PropagateDependencies();

        //! map node class names to creator functions
        typedef std::map<GenICam::gcstring,CreateFunc_t> CreatorMap_t;
        
        //! Get the instance of the node creator map
        static CreatorMap_t* GetInstance();

        //! Map holding pointers to nodes referencey by their names
        //typedef std::map<GenICam::gcstring, INodePrivate*> string2node_t;

        //! Map holding nodes keyed by their name
        string2node_t& Map();
        const string2node_t& Map() const;
        void ClearAllNodes();
        string2node_t *m_pMap;

        //! Creates a node of given type on the heap and registers it in the map
        INodePrivate *CreateAndRegisterNode(GenICam::gcstring &NodeType, GenICam::gcstring &NodeName, GenICam::gcstring &NodeNameSpace);

        //! The name of the device
        GenICam::gcstring m_Name;

        //! indicates if the nodemap is initilizaed
        bool m_Initialized;

        //! counts the depth of SetValue call-chains
        Counter m_Bathometer;

        //! List of pointers to nodes which need to be polled
        NodeSet_t m_PollingNodes;


    private:
        CNodeMap(const CNodeMap&);
        CNodeMap& operator=(const CNodeMap&);

        mutable ThreadingModel::LockType m_Lock;
    };
}

#ifdef _MSC_VER // *JS*
#pragma warning(pop)
#endif

#endif // GENAPI_NODEMAP_H
