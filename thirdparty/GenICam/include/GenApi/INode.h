//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/INode.h,v 1.25 2008/01/17 16:54:03 hartmut_nebelung Exp $
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
\brief  Definition of interface INode and types NodeList_t and CallbackHandleType:
\ingroup GenApi_PublicInterface
*/


#ifndef GENAPI_INODE_H
#define GENAPI_INODE_H

#include "GenApiDll.h"
#include "Types.h"
#include "IBase.h"
#include <Base/GCString.h>
#include "Container.h"

#include <assert.h>

#pragma warning ( push )
#pragma warning ( disable : 4251 ) // XXX needs to have dll-interface to be used by clients of class YYY

namespace GenApi
{

    interface INode;
    interface INodeMap;

    //! a list of node references
    typedef node_vector NodeList_t;

    //! the callback handle for nodes
    typedef intptr_t CallbackHandleType;

    class NodeCallback;

    //*************************************************************
    // INode interface
    //*************************************************************

    /**
    \brief Interface common to all nodes
    \ingroup GenApi_PublicInterface
    */
    interface INode  : virtual public IBase
    {
        //! Get node name
        virtual GenICam::gcstring GetName(bool FullQualified=false) const =  0;

        //! Get name space
        virtual GenApi::ENameSpace GetNameSpace() const =  0;

        //! Get the recommended visibility of the node
        virtual EVisibility GetVisibility() const  = 0;

        //! Set/reset changed status
        virtual void InvalidateNode() = 0;

        //! Is the node value cachable
        virtual bool IsCachable() const =  0;

        //! Get Caching Mode
        virtual ECachingMode GetCachingMode() const = 0;

        //! recommended polling time (for not cachable nodes)
        virtual int64_t GetPollingTime() const = 0;

        //! Get a short description of the node
        virtual GenICam::gcstring GetToolTip() const = 0;

        //! Get a long description of the node
        virtual GenICam::gcstring GetDescription() const = 0;

        //! Get a name string for display
        virtual GenICam::gcstring GetDisplayName() const = 0;

        //! Get all children of the node
        virtual void GetChildren(NodeList_t &Children) const =  0;

        //! Register change callback
        virtual CallbackHandleType RegisterCallback( NodeCallback& ) = 0;

        //! Deregister change callback
        virtual CallbackHandleType DeregisterCallback( CallbackHandleType )=0;

        //! Retrieves the central node map 
        virtual INodeMap* GetNodeMap() const = 0;

        //! Get the EventId of the node
        virtual GenICam::gcstring GetEventID() const =  0;

        //! True if the node is streamable
        virtual bool IsStreamable() const =  0;

        //! Returns a list of the names all properties set during initialization
        virtual void GetPropertyNames(GenICam::gcstring_vector &PropertyNames) const =  0;

        //! Retrieves a property plus an additional attribute by name
        /*! If a property has multiple values/attribute they come with Tabs as delimiters */
        virtual bool GetProperty(const GenICam::gcstring& PropertyName, GenICam::gcstring& ValueStr, GenICam::gcstring& AttributeStr) = 0;

        //! Imposes an access mode to the natural access mode of the node
        virtual void ImposeAccessMode(EAccessMode ImposedAccessMode) =  0;

        //! Imposes a visibility  to the natural visibility of the node
        virtual void ImposeVisibility(EVisibility ImposedVisibility) =  0;

        //! Retrieves the a node which describes the same feature in a different way
        virtual INode* GetAlias() const = 0;

    };

    //! \addtogroup GenApi_PublicInterface
    //! \{

    //! Tests if readable
    inline bool IsReadable( EAccessMode AccessMode ) 
    {
        return RO == AccessMode || RW == AccessMode;
    }

    //! Checks if a node is readable
    inline bool IsReadable( const IBase* p)
    {
        return IsReadable( p->GetAccessMode() );
    }

    //! Checks if a node is readable
    inline bool IsReadable( const IBase& r)
    {
        return IsReadable( r.GetAccessMode() );
    }

    //! Tests if writable
    inline bool IsWritable( EAccessMode AccessMode ) 
    {
        return WO == AccessMode || RW == AccessMode;
    }

    //! Checks if a node is writable
    inline bool IsWritable( const IBase* p)
    {
        return IsWritable( p->GetAccessMode() );
    }

    //! Checks if a node is writable
    inline bool IsWritable( const IBase& r)
    {
        return IsWritable( r.GetAccessMode() );
    }

    //! Tests if implemented
    inline bool IsImplemented( EAccessMode AccessMode ) 
    {
        return AccessMode != NI;
    }

    //! Checks if a node is implemented
    inline bool IsImplemented( const IBase* p)
    {
        return IsImplemented(p->GetAccessMode() );
    }

    //! Checks if a node is implemented
    inline bool IsImplemented( const IBase& r)
    {
        return IsImplemented(&r);
    }

    //! Tests if available
    inline bool IsAvailable( EAccessMode AccessMode ) 
    {
        return ! ( AccessMode == NA || AccessMode == NI );
    }
    //! Checks if a node is available
    inline bool IsAvailable( const IBase* p)
    {
        return IsAvailable(p->GetAccessMode() );
    }
    //! Checks if a node is available
    inline bool IsAvailable( const IBase& r)
    {
        return IsAvailable(r.GetAccessMode() );
    }

    //! Computes which access mode the two guards allow together
    inline EAccessMode Combine(EAccessMode Peter, EAccessMode Paul)
    {
        assert( Peter != _UndefinedAccesMode );
        assert( Paul != _UndefinedAccesMode );

        if( Peter == NI || Paul == NI )
            return NI;
        else if( Peter == NA || Paul == NA )
            return NA;
        else if( (Peter == RO && Paul == WO) || (Peter == WO && Paul == RO)  )
            return NA;
        else if( Peter == WO || Paul == WO )
            return WO;
        else if( Peter == RO || Paul == RO )
            return RO;
        else 
            return RW;
    }


    //! Tests Visibility
    /*! CAVE : this relys on the EVisibility enum's coding */
    inline bool IsVisible( EVisibility Visibility, EVisibility MaxVisiblity ) 
    {
        return (Visibility <= MaxVisiblity);
    }


    //! Computes which visibility the two guards allow together
    inline EVisibility Combine(EVisibility Peter, EVisibility Paul)
    {
        assert( Peter != _UndefinedVisibility );
        assert( Paul != _UndefinedVisibility );

        if( Peter == Invisible || Paul == Invisible )
            return Invisible;
        else if( Peter == Guru || Paul == Guru )
            return Guru;
        else if( Peter == Expert || Paul == Expert )
            return Expert;
        else 
            return Beginner;
    }


    //! \}

}

#pragma warning ( pop )

#endif // ifndef GENAPI_INODE_H
