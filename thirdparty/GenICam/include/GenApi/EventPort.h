//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  GenApi
//    Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/EventPort.h,v 1.12 2008/01/16 13:36:07 hartmut_nebelung Exp $
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
\brief    Declaration of the CEventPort class.
\ingroup GenApi_PublicUtilities
*/

#ifndef GENAPI_EVENTPORT_H
#define GENAPI_EVENTPORT_H

#pragma warning(push)
#pragma warning ( disable : 4068 ) // unknown pragma; refers to BullsEyeCoverage
#include "GenApi.h"

namespace GenApi
{

    /**
    \brief Port attachable to an event
    \ingroup GenApi_PublicUtilities
    */
    class GENAPI_DECL CEventPort : public IPortConstruct
    {

    public:
        //! Constructor; can attach to a node
        CEventPort(INode* pNode = NULL);

        //! Destructor; detaches from the port
        ~CEventPort(); 

        //-------------------------------------------------------------
        // IPortConstruct implementation
        //-------------------------------------------------------------

        //!    Get    the    access mode    of the node
        virtual    EAccessMode    GetAccessMode()    const;

        //! Reads a chunk of bytes from the port
        virtual void Read(void *pBuffer, int64_t Address, int64_t Length);    
        
        //! Writes a chunk of bytes to the port
        virtual void Write(const void *pBuffer, int64_t Address, int64_t Length);

        //! Called from the port node to give the chunk port a pointer to itself
        virtual void SetPortImpl(GenApi::IPort* pPort);
        
        //! Determines if the port adapter must perform an endianess swap
        #pragma BullseyeCoverage off
        virtual EYesNo GetSwapEndianess()
        {
            return No;
        }
        #pragma BullseyeCoverage on

        //---------------------------------------------------------------
        // Implementation 
        //---------------------------------------------------------------

        // Invalidates the chunk port node
        void InvalidateNode();

        //-------------------------------------------------------------
        // Initializing
        //-------------------------------------------------------------

        //! Attaches to the Node
        bool AttachNode(GenApi::INode* pNode);

        //! Detaches from the Node
        void DetachNode();

        //! Gets the EventID length 
        int GetEventIDLength()
        {
            return m_EventIDLength;
        }

        //! Checks if a ChunkID matches
        bool CheckEventID(uint8_t* pEventIDBuffer, int EventIDLength);

        //! Attaches the an Event to the EventPort
        void AttachEvent(uint8_t *pBaseAddress, int64_t Length);

        //! Detaches the Event from the EventPort
        void DetachEvent();


    protected:

        //-------------------------------------------------------------
        // Member variables
        //-------------------------------------------------------------

        //! Pointer to the begin of the buffer
        uint8_t *m_pEventData;

        //! Length of the chunk
        int64_t m_EventDataLength;
#pragma warning( push )
#pragma warning( disable: 4251 )
        //! Pointer to the node holding a reference to this implementation
        CNodePtr m_ptrNode;
#pragma warning( pop )

        //! Binary version of the EventID
        uint8_t *m_pEventIDBuffer;

        //! Length of the EventID buffer
        int m_EventIDLength;
    };

}
#pragma warning(pop)
#endif // GENAPI_EVENTPORT_H
