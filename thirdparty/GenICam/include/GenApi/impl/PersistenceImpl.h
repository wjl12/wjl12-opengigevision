//-----------------------------------------------------------------------------
//  (c) 2007 by National Instruments
//  Project: GenApi
//  Author:  Eric Gross
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/PersistenceImpl.h,v 1.1 2007/11/13 19:57:41 hartmut_nebelung Exp $
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
\brief    Definition of template CPointer
\ingroup GenApi_PublicUtilities
*/

#ifndef _GENICAM_PERSISTENCEIMPL_H
#define _GENICAM_PERSISTENCEIMPL_H

#include <GenApi/Types.h>
#include <GenApi/Pointer.h>
#include <GenApi/GenApiVersion.h>
#include <GenApi/Persistence.h>
#include <GenICam.h>
#include <list>
#include <iostream>

using namespace GenICam;

namespace GenApi
{

    //!  IIterator interface - a generic way to iterate the values of a node
    class IIterator {
        //! Resets the interator
        virtual void IterateReset() = 0;

        //! Advances the interator to the next element
        virtual void IterateIncrement() = 0;

        //! True if no more elements to interate are available
        virtual bool IterateIsDone() const = 0;

        //! Computes the maximum number of elements the interator can visit
        virtual int64_t IterateGetMaxIteratons() const = 0;  
    };


    //! Iterator which can walk the interfaces which are valid for selectors
    class IIteratorWrapper : public IIterator 
    {
    public:
        //! Constructor  
        IIteratorWrapper(IValue* node);

        //! Destructor
        ~IIteratorWrapper();

        //! Resets the interator
        virtual void IterateReset();

        //! Advances the interator to the next element
        virtual void IterateIncrement();

        //! True if no more elements to interate are available
        virtual bool IterateIsDone() const;
        
        //! Computes the maximum number of elements the interator can visit
        virtual int64_t IterateGetMaxIteratons() const;

    private:
        //! holds a poiinter if the node to interate is an integer
        IInteger* interfaceIIntegerPtr;

        //! holds a poiinter if the node to interate is an enumeration
        IEnumeration* interfaceIEnumPtr;

        //! holds a poiinter if the node to interate is an boolean
        IBoolean* interfaceIBoolPtr;    
    };


    //! Generates a list of nodes that will be persisted
    void GetNodeListForPersistence(INodeMap *pNodeMap, FeatureList_t& nodeList, INode* recurseNode = NULL);

    //! Helper function to assist iterating a group of selectors for one or more features
    int64_t RecurseSelectors(IPersistScript& persistScript, FeatureList_t& featuresToPersist, FeatureList_t& selectors, FeatureList_t::iterator recursionSelector);

    //! Persists a node map into an IPersistScript that can be played back at some future time
    void PersistFeatures(INodeMap *pNodeMap, IPersistScript& persistScript); 

}

#endif //_GENICAM_PERSISTENCEIMPL_H

