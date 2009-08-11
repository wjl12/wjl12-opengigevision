//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  GenApi
//  Author:   Hartmut Nebelung
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/EventAdapter1394.h,v 1.2 2007/08/16 13:56:38 hartmut_nebelung Exp $
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
/**
\file
\brief    Declaration of the CEventAdapter1394 class.
*/
#ifndef __EVENTADAPTER1394_H__
#define __EVENTADAPTER1394_H__


#if _MSC_VER > 1000
#pragma once
#endif

#include "EventAdapter.h"
#include "Log.h"

namespace GenApi
{
    // the opaque event data on IEEE 1394
    struct EventData1394;

    //! Distribute the events to the node map
    class GENAPI_DECL CEventAdapter1394 : public GenApi::CEventAdapter
    {
    public:
        //! constructor
        explicit CEventAdapter1394( INodeMap* pNodeMap=NULL );
    private:
        //! not implemented: copy constructor
        CEventAdapter1394( const CEventAdapter1394& );
        //! not implemented: assignment operator
        CEventAdapter1394& operator=( const CEventAdapter1394& );

    public:
        virtual void DeliverMessage( const uint8_t msg[], uint32_t numBytes );
        //! distributes events to node map
        void DeliverEventMessage( EventData1394& Event, uint32_t numBytes );
    private:
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
    };
}
#endif //__EVENTADAPTER1394_H__
