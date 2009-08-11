//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Alexander Happe
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/NodeT.h,v 1.11 2007/08/16 13:56:25 hartmut_nebelung Exp $
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
\brief    Definition of the NodeT class template
*/

#ifndef GENAPI_NODET_H
#define GENAPI_NODET_H

#include "../INode.h"
#include "../Synch.h"

namespace GenApi
{

    /**
    \brief Implementation of the INode interface
    */
    template< class Base >
    class GENAPI_DECL NodeT : public Base
    {
    public:
        //! Implementation of INode::GetName()
        virtual GenICam::gcstring GetName(bool FullQualified) const
        {
            AutoLock l(Base::GetLock());

            return Base::InternalGetName(FullQualified);
        }

        //! Implementation of INode::GetNameSpace()
        virtual GenApi::ENameSpace GetNameSpace() const
        {
            AutoLock l(Base::GetLock());

            return Base::InternalGetNameSpace();
        }

        //! Implementation of INode::GetVisibility()
        virtual EVisibility GetVisibility() const 
        {
            AutoLock l(Base::GetLock());

            EVisibility NaturalVisibility = Base::InternalGetVisibility();

            EVisibility Visibility = Combine(NaturalVisibility, Base::m_ImposedVisibility);

            return Visibility;
        }

        //! Implementation of INode::InvalidateNode()
        virtual void InvalidateNode() 
        {
            AutoLock l(Base::GetLock());

            Base::InternalInvalidateNode();
        }

        //! Implementation of INode::IsCachable()
        virtual bool IsCachable()const 
        {
            AutoLock l(Base::GetLock());

            return Base::InternalIsCachable();
        }

        //! Implementation of INode::GetCachingMode
        virtual ECachingMode GetCachingMode() const 
        {
            AutoLock l(Base::GetLock());

            return Base::InternalGetCachingMode();
        }

        //! Implementation of INode::GetPollingTime()
        virtual int64_t GetPollingTime()const 
        {
            AutoLock l(Base::GetLock());

            return Base::InternalGetPollingTime();
        }

        //! Implementation of INode::GetToolTip()
        virtual GenICam::gcstring GetToolTip() const
        {
            AutoLock l(Base::GetLock());

            return Base::InternalGetToolTip();
        }

        //! Implementation of INode::GetDescription()
        virtual GenICam::gcstring GetDescription() const
        {
            AutoLock l(Base::GetLock());

            return Base::InternalGetDescription();
        }

        //! Implementation of INode::GetDisplayName
        virtual GenICam::gcstring GetDisplayName() const 
        {
            AutoLock l(Base::GetLock());

            return Base::InternalGetDisplayName();
        }

        //! Implementation of INode::GetChildren()
        virtual void GetChildren(NodeList_t &Children) const 
        {
            AutoLock l(Base::GetLock());

            return Base::InternalGetChildren( Children );
        }

        //! Implementation of INode::RegisterCallback()
        virtual CallbackHandleType RegisterCallback( NodeCallback& ncb ) 
        {
            AutoLock l(Base::GetLock());

            return Base::InternalRegisterCallback( ncb );
        }

        //! Implementation of INode::DeregisterCallback()
        virtual CallbackHandleType DeregisterCallback( CallbackHandleType ch )
        {
            AutoLock l(Base::GetLock());

            return Base::InternalDeregisterCallback( ch );
        }

        //! Implementation of INode::GetNodeMap()
        virtual INodeMap* GetNodeMap() const 
        {
            AutoLock l(Base::GetLock());

            return Base::InternalGetNodeMap();
        }

        //! Implementation of INode::GetNodeMap()
        virtual GenICam::gcstring GetEventID() const
        {
            AutoLock l(Base::GetLock());

            return Base::GetEventID();
        }

        //! Returns a list of the names all properties set during initialization
        virtual void GetPropertyNames(GenICam::gcstring_vector &PropertyNames) const
        {
            AutoLock l(Base::GetLock());

            Base::GetPropertyNames(PropertyNames);
        }

        //! Retrieves a property plus an additional attribute by name
        /*! If a property has multiple values/attribute they come with Tabs as delimiters */
        virtual bool GetProperty(const GenICam::gcstring& PropertyName, GenICam::gcstring& ValueStr, GenICam::gcstring& AttributeStr)
        {
            AutoLock l(Base::GetLock());

            return Base::GetProperty(PropertyName, ValueStr, AttributeStr);
        }

    };


}

#endif // GENAPI_NODET_H
