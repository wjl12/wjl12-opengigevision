//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Hartmut Nebelung
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/Callback.h,v 1.9.2.1 2008/07/14 08:30:46 hartmut_nebelung Exp $
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
\brief    Definition of template Base_Callback1Body and Callback1
\ingroup GenApi_PublicImpl

Defines a bunch of templates for creating callback-functors taking 1 Parameter 
(=> Callback1) and returning void.

\note does not work with all stl containers, i.e. std::map

*/

#ifndef GENAPI_CALLBACK_H
#define GENAPI_CALLBACK_H

#include <cassert>

namespace GenApi
{
    /**
    \brief abstract callback container
    \ingroup GenApi_PublicImpl
    */
    template <class P1>
    class Base_Callback1Body
    {
    public:
        //! destructor
        virtual ~Base_Callback1Body<P1>()
        {};
        
        //! do operation with P1 as argument
        virtual void operator()(P1) const = 0;
        
        //! deep copy
        virtual Base_Callback1Body<P1>* clone() const = 0;
    };

    /**
    \brief callback object with one parameter P1
    \ingroup GenApi_PublicImpl
    */
    template<class P1>
    class GENAPI_DECL Callback1
    {
    public:

        //! constructor, taking lifetime control of body
        Callback1( Base_Callback1Body<P1>* pBody) : 
            m_pBody(pBody) 
        {
        }

        //! copy constructor doing deep copy
        Callback1(const Callback1<P1>& callback) : 
            m_pBody(callback.m_pBody ? callback.m_pBody->clone() : NULL) 
        {
        }

        //! destructor, destroying body
        ~Callback1() 
        {    
            delete m_pBody; 
            m_pBody = NULL;
        }

        //! assignment operator
        Callback1<P1>& operator=(const Callback1<P1>& callback)
        {
            if (this != &callback)
            {
                delete m_pBody;
                m_pBody =  callback.m_pBody ? callback.m_pBody->clone() : NULL;
            }
            return *this;
        }

        //! do operation defined in body
        void operator()(P1 p1) 
        {
            if (m_pBody)
                (*m_pBody)(p1);
            else
                assert( false && "Callback1:Empty body" );
        }

        bool HasValidBody() const 
        {
            return m_pBody != NULL;
        }
    
    private:
        Base_Callback1Body<P1> *m_pBody;
    };


}

#endif // ifndef GENAPI_CALLBACK_H
