//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  GenApi
//    Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/EventAdapter.h,v 1.8 2007/08/16 13:56:38 hartmut_nebelung Exp $
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
\brief    Declaration of the CEventAdapter class.
\ingroup GenApi_PublicUtilities
*/

#ifndef GENAPI_EVENTADAPTER_H
#define GENAPI_EVENTADAPTER_H

#include "Types.h"
#include "INodeMap.h"
#include "EventPort.h"

#include <vector>

namespace GenApi
{

    /**
    \brief Delivers Events to ports
    \ingroup GenApi_PublicUtilities
    */
    class GENAPI_DECL CEventAdapter 
    {

    public:
        //! Constructor
        CEventAdapter(INodeMap* pNodeMap = NULL);

        //! Destructor
        virtual ~CEventAdapter();

        //! Attaches to a node map and retrieves the chunk ports
        virtual void AttachNodeMap(INodeMap* pNodeMap);

        //! Detaches from the node emap 
        virtual void DetachNodeMap();

        //! Deliver message
        virtual void DeliverMessage( const uint8_t msg[], uint32_t numBytes ) = 0;
        

    protected:

        //! An array of attached ChunkPorts 
#pragma warning( push )
#pragma warning( disable: 4251 )
        std::vector<CEventPort*> m_pEventPorts;
#pragma warning( pop )
    };

}

#endif // GENAPI_EVENTADAPTER_H
