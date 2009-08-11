//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/Node.h,v 1.26 2007/12/18 17:10:15 hartmut_nebelung Exp $
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
\brief    Definition of CNodeImpl
*/

#ifndef GENAPI_NODE_H
#define GENAPI_NODE_H

#include <sstream>
#include <cctype>

#include <Base/GCException.h>
#include "../GenApiDll.h"
#include "../Synch.h"
#include "../Types.h"
#include "../INode.h"
#include "../ICategory.h"
#include "INodePrivate.h"
#include "INodeMapPrivate.h"
#include "../IInteger.h"
#include "../IBoolean.h"
#include "../ISelector.h"
#include "../NodeCallback.h"
#include "NodeMacros.h"
#include "../EnumClasses.h"
#include "../Log.h"
#include "Value2String.h"

#ifdef _MSC_VER // *JS*
#pragma warning(push)
#pragma warning(disable: 4251) // class 'xxx' needs to have dll-interface to be used by clients of class 'yyy'
#endif

namespace GenApi
{
    struct CallbackController;
    struct CallbackType;
    class CNodeImpl;


    //*************************************************************
    // CBoolInt class
    //*************************************************************

    //! Helper class making IInteger and IBoolean look like IBoolean
    class CBoolInt
    {
    public:
        //! Constructor
        CBoolInt() : m_pInteger(NULL), m_pBoolean(NULL)
        {}

        //! Assignment
        void operator=( IBase *pT ) 
        {
            m_pInteger = dynamic_cast<IInteger*>(pT);
            m_pBoolean = dynamic_cast<IBoolean*>(pT);
        }

        // this method is currently not used
        #if 0
        //! Set a value to either bool or int
        void SetValue( bool Value )
        {
            if(m_pInteger)
                m_pInteger->SetValue( Value ? 1 : 0 );
            else if(m_pBoolean)
                m_pBoolean->SetValue( Value );
            else 
                throw LOGICAL_ERROR_EXCEPTION( "Cannot dereference NULL pointer" );
        }
        #endif

        //! Get a value from either bool or int
        bool GetValue() const
        {
            if(m_pInteger)
                return m_pInteger->GetValue() == 0 ? false : true;
            #pragma BullseyeCoverage off
            else if(m_pBoolean)
                return m_pBoolean->GetValue();
            else 
                throw LOGICAL_ERROR_EXCEPTION( "Cannot dereference NULL pointer" );
            #pragma BullseyeCoverage on
        }

    protected:
        //! holds the pointer if it is an IInteger
        IInteger *m_pInteger;

        //! holds the pointer if it is an IBoolean
        IBoolean *m_pBoolean;
    };

    //*************************************************************
    // CNodeImpl class
    //*************************************************************

    /**
    \brief Standard INode implementation
    */
    class GENAPI_DECL CNodeImpl : virtual public INodePrivate, public ISelector
    {
    public:
        //! Constructor
        CNodeImpl();

        //! Destructor
        virtual ~CNodeImpl();

        //-------------------------------------------------------------
        // IBase implementation
        //-------------------------------------------------------------
    protected:

        //! Get the access mode of the node
        virtual EAccessMode InternalGetAccessMode() const;

        //-------------------------------------------------------------
        // INode implementation
        //-------------------------------------------------------------

        // Get node name
        virtual GenICam::gcstring InternalGetName(bool FullQualified=false) const;

        //! Get name space
        virtual GenApi::ENameSpace InternalGetNameSpace()const;

        //! Get the recommended visibility of the node
        virtual EVisibility InternalGetVisibility() const;

        // Invalidate the node
        virtual void InternalInvalidateNode();

        //! Is the node value cachable
        virtual bool InternalIsCachable() const;

        //! Get Caching Mode
        virtual ECachingMode InternalGetCachingMode() const;

        //! recommended polling time (for not cachable nodes)
        virtual int64_t InternalGetPollingTime() const;

        //! Get a short description of the node
        virtual GenICam::gcstring InternalGetToolTip() const;

        //! Get a long description of the node
        virtual GenICam::gcstring InternalGetDescription() const;

        // Get node display name
        virtual GenICam::gcstring InternalGetDisplayName() const;

        //! Get all children of the node
        virtual void InternalGetChildren(GenApi::NodeList_t &Children) const;

        //! Register change callback
        virtual CallbackHandleType InternalRegisterCallback( GenApi::NodeCallback &cb );

        //! Deregister change callback
        virtual CallbackHandleType InternalDeregisterCallback( CallbackHandleType h );

        //! Retrieves the node map
        virtual INodeMap* InternalGetNodeMap() const;

        //! Get the EventId of the node
        virtual GenICam::gcstring GetEventID() const;

        //! True if the node is streamable
        virtual bool IsStreamable() const;

        //! Returns a list of the names all properties set during initialization
        virtual void GetPropertyNames(GenICam::gcstring_vector &PropertyNames) const;

        //! Retrieves a property plus an additional attribute by name
        /*! If a property has multiple values/attribute they come with Tabs as delimiters */
        virtual bool GetProperty(const GenICam::gcstring& PropertyName, GenICam::gcstring& ValueStr, GenICam::gcstring& AttributeStr);

        //! Imposes an access mode to the natural access mode of the node
        virtual void ImposeAccessMode(EAccessMode ImposedAccessMode);

        //! Imposes a visibility  to the natural visibility of the node
        virtual void ImposeVisibility(EVisibility ImposedVisibility);

        //! Retrieves the a node which describes the same feature in a different way
        virtual INode* GetAlias() const;

    public:
        //-------------------------------------------------------------
        // INodePrivate implementation
        //-------------------------------------------------------------

        // Fire the on-change callbacks
        virtual void FireCallBacks(bool allDependents);

        // Initializes the object
        virtual void FinalConstruct();


        // Called when node is about to be updated
        virtual void SetInvalid(ESetInvalidMode simMode);

        // Registers the node
        virtual void Register(GenApi::INodeMapPrivate* const pNodeMap, const char *pNodeType, const char *pName, const char *pNameSpace);

        // Create the set of dependent nodes
        virtual bool PropagateDependency( NodePrivateList_t &Dependents );

        // Create the set of terminal nodes
        virtual void PropagateTerminals( GenApi::NodeSet_t &Terminals, NodePrivateList_t &Visited );

        // Returns the set of parents
        virtual void GetParents(GenApi::NodeList_t &) const;

        // Returns true, if the node is a terminal node, i.e., a node which has to trigger the on-change callbacks
        virtual bool IsTerminalNode() const;

        // Returns the terminal nodes for this node
        void GetTerminalNodes( GenApi::NodeList_t& Terminals ) const;

        //! Returns true, if the node is a top-level node, i.e, having no parents
        virtual bool IsTopLevelNode() const;

        //! Convert map (contains SetProperty)
        BEGIN_CONVERT_MAP
            CONVERT_STRING_ENTRY(NodeType, m_NodeType)
            CONVERT_STRING_ENTRY(Name, m_Name)
            CONVERT_ENUM_ENTRY(NameSpace, m_NameSpace, ENameSpaceClass)    
            CONVERT_STRING_ENTRY(DeviceName, m_DeviceName)
            CONVERT_STRING_ENTRY(DisplayName, m_DisplayName)
            CONVERT_STRING_ENTRY(ToolTip, m_ToolTip)
            CONVERT_STRING_ENTRY(Description, m_Description)
            CONVERT_ENUM_ENTRY(Visibility, m_Visibility, EVisibilityClass)
            CONVERT_NODE_REFERENCE(pIsImplemented, m_pIsImplemented, IValue)
            CONVERT_NODE_REFERENCE(pIsAvailable, m_pIsAvailable, IValue)
            CONVERT_NODE_REFERENCE(pIsLocked, m_pIsLocked, IValue)
            CONVERT_NODE_REFERENCE(pAlias, m_pAlias, INode)
            CONVERT_ENUM_ENTRY(Cachable, m_CachingMode, ECachingModeClass)
            CONVERT_NODELIST_REFERENCE(pFeature, m_Features, IValue, FeatureList_t)
            CONVERT_NODELIST_REFERENCE2(pSelecting, m_Selecting, IValue)
            CONVERT_STRING_ENTRY(EventID, m_EventID)
            CONVERT_ENUM_ENTRY(Streamable, m_IsStreamable, EYesNoClass)
            CONVERT_NODELIST_REFERENCE2(pSelected, m_Selected, IValue)
            CONVERT_ENUM_ENTRY(ImposedAccessMode, m_ImposedAccessMode, EAccessModeClass)
            CONVERT_ENUM_ENTRY(ImposedVisibility, m_ImposedVisibility, EVisibilityClass)
            CONVERT_ENTRY(PollingTime, m_PollingTime)
        END_CONVERT_MAP

    protected:
        //-------------------------------------------------------------
        // Initializing
        //-------------------------------------------------------------

    public:

        //! Inserts a reference to the children list
        void AddChild( INodePrivate* pNode );

        //! Inserts a reference to the children list
        void AddChild( INode* pNode);

    protected:
        //! Inserts a refernece to the parent list
        void AddParent( INodePrivate* pNode);

        //! Invalidates all nodes which will become affected by a SetValue call into the node tree
        void PreSetValue();

        //! Fires callback on all nodes which became affected by a SetValue call into the node tree
        void PostSetValue();

        //! The name of the node
        GenICam::gcstring m_Name;

        //! The device name of the node tree
        GenICam::gcstring m_DeviceName;

        //! The display name string of the node
        GenICam::gcstring m_DisplayName;

        //! The type of the node, e.g. MaskedIntReg 
        GenICam::gcstring m_NodeType;

        //! The namespace of the node 
        ENameSpace m_NameSpace;

        //! The ToolTip for the node
        GenICam::gcstring m_ToolTip;

        //! The Description of the node
        GenICam::gcstring m_Description;

        //! Pointer to the object holding the hash map with all nodes
        INodeMapPrivate*  m_pNodeMap;

        //! Pointer to a Node, which indicates if the node is implemented
        IValue *m_pIsImplemented;
        
        //! Pointer to a Node, which indicates if the node is implemented
        CBoolInt m__pIsImplemented;

        //! Pointer to a Node, which indicates if the node is available
        IValue *m_pIsAvailable;

        //! Pointer to a Node, which indicates if the node is available
        CBoolInt m__pIsAvailable;

        //! Pointer to a Node, which indicats if the node is locked (i.e. not writable)
        IValue *m_pIsLocked;

        //! Pointer to a Node, which indicats if the node is locked (i.e. not writable)
        CBoolInt m__pIsLocked;

        //! Pointer to a Node, which describes the same feature as this one
        INode *m_pAlias;

        //! recommended visibility;
        EVisibility m_Visibility;

        //! Access mode imposed on the natural access mode of the node
        EAccessMode m_ImposedAccessMode;

        //! Visibility imposed to the natural viibility of the node
        EVisibility m_ImposedVisibility;

        //! List of pointers to the children of this node
        NodeSet_t m_Children;

        //! List of dependent nodes
        NodeSet_t m_Dependents;

        //! List of parents
        NodeSet_t m_Parents;

        //! List of terminal nodes
        NodeSet_t m_Terminals;

        //! List of childs which could be terminals
        NodeSet_t m_TerminalCandidates;

        //! Class handling the callbacks
        typedef CallbackController* CallbackControllerType;

        //! Handles the callbacks
        CallbackControllerType m_CallbackController;

        //! indicates that the node has changed
        mutable ECachingMode m_CachingMode;

        //! indicates that the CachingMode has bee initialized
        mutable bool m_IsCachingModeInitialized;

        //! recommended polling time in [ms]
        int64_t m_PollingTime;

        //! time elapsed since the last poll
        int64_t m_ElapsedTime;

        //! cache access mode
        mutable EAccessMode m_AccessModeCache;

        //! Default implementation of GetAccessMode
        EAccessMode InternalGetAccessMode(IBase* pValue) const;

        //! Creates the full qualified name
        GenICam::gcstring GetQualifiedName(GenICam::gcstring Name, ENameSpace NameSpace) const;

        //! List of node refererences to features
        FeatureList_t m_Features;

        //! \name ISelector interface implementation
        // \{
        // List of selected features
        FeatureList_t m_Selected;

        // List of seleting features
        FeatureList_t m_Selecting;

        //! true iff this feature selects a group of features
        virtual bool IsSelector() const;

        // Get a list of features selected by this selector
        virtual void GetSelectedFeatures( FeatureList_t& list ) const;

        //! retrieve the group of features selecting this node
        virtual void GetSelectingFeatures( FeatureList_t& ) const;

        // \}

        //! true if the valeu cache is valid
        mutable bool m_ValueCacheValid;

        //! The EventID 
        GenICam::gcstring m_EventID;

        //! inidcates ifthe node is streamamble
        EYesNo m_IsStreamable;

        //! A list with all properties set
        GenICam::gcstring_vector m_PropertyNames;

        /*---------------------------------------------------------------*/
        // Loggers
        /*---------------------------------------------------------------*/

        // for safety reasons please keep these member varibles at the 
        // end of the class' memory layout
#ifdef LOG4C
        //! Logger for messages concerning the AccessMode
        log4cpp::Category *m_pAccessLog;

        //! Logger for messages concerning the getting and setting values
        log4cpp::Category *m_pValueLog;

        //! Logger for messages concerning the range check
        log4cpp::Category *m_pRangeLog;

        //! Logger for messages concerning the port access
        log4cpp::Category *m_pPortLog;

        //! Logger for messages concerning the caching access
        log4cpp::Category *m_pCacheLog;

        //! Logger for messages concerning administrative access, e.g. GetNode()
        log4cpp::Category *m_pAdminLog;

        //! Logger for messages concerning miscellaneoud access which does not fit to the other categories
        log4cpp::Category *m_pMiscLog;
#endif

        // shortcut for the lock type
        typedef AutoLock Lock;  

        // Acquire central lock
        ThreadingModel::LockType& GetLock() const
        {
            assert( m_pNodeMap != NULL );

            return m_pNodeMap->GetLock();
        }

        //! The bathometer is a counter used to measure the depth of SetValue-like call chains
        Counter& GetBathometer()
        {
            assert( m_pNodeMap);
            
            return m_pNodeMap->GetBathometer();
        }

        // Get the dependent nodes
        void GetDependencies( GenApi::NodeList_t &Dependents ) const
        {
            Lock l(GetLock());
            // increase size first in case of a memory exception
            Dependents.reserve( m_Dependents.size() );
            Dependents.clear();
            NodeSet_t::const_iterator it;
            for (it=m_Dependents.begin(); it!=m_Dependents.end(); ++it)
            {
                Dependents.push_back( *it );
            }
        }

        //! Invalidates the node if the polling time has elapsed
        virtual void Poll( int64_t ElapsedTime );


        protected:
        //! Used to ensure that PostSetValue() is called in any case 
        struct PostSetValueFinalizer { 
            PostSetValueFinalizer(CNodeImpl* pThis) : m_pThis(pThis) {} 
            ~PostSetValueFinalizer() {
                try { m_pThis->PostSetValue(); } 
                #pragma BullseyeCoverage off
                catch ( ... ) 
                {
                    // don't throw exceptions here, because the dtor might be 
                    // called when unwinding the stack. 
                } 
                #pragma BullseyeCoverage on
            }
            CNodeImpl* m_pThis; 
        };
        

    
    };



}

#ifdef _MSC_VER // *JS*
#pragma warning(pop)
#endif

#endif // ifndef GENAPI_NODE_H
