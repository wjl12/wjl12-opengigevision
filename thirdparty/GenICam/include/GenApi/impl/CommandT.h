//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Alexander Happe
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/CommandT.h,v 1.8.2.1 2008/05/30 15:16:12 hartmut_nebelung Exp $
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
\brief    Definition of CommandT class template
*/

#ifndef GENAPI_COMMANDT_H
#define GENAPI_COMMANDT_H


namespace GenApi
{

    /**
        \brief Implementation of the ICommand interface
    */
    template <class Base>
    class GENAPI_DECL CommandT : public Base
    {
    public:
        //! Execute the command
        virtual void Execute()
        {
            AutoLock l(Base::GetLock());
            #ifdef LOG4C    
                Counter& c(Base::GetBathometer()); 
                if( 0 == c )
                    Base::m_pValueLog->info(">>>>" );

                Base::m_pValueLog->info("Execute()");
            #endif

            if( !IsWritable( this ) )
                throw ACCESS_EXCEPTION("Node is not executable"); 


            {
                typename Base::PostSetValueFinalizer PostSetValueCaller(this);  // dtor calls Base::PostSetValue

                Base::PreSetValue(); // invalidates all nodes if this is the first call in a chain of SetValue-like calls
                Base::InternalExecute();

                // now Base::PostSetValue is called by ~PostSetValueFinalizer, i.e. updates are fired
                // if this is the call on the top of a chain of SetValue calls
            }

            #ifdef LOG4C            
                if( 0 == c )
                    Base::m_pValueLog->info("<<<<" );
            #endif
        }

        //! Execute the command
        virtual void operator()()
        {
            Execute();
        }

        //! Query whether the command is executed
        virtual bool IsDone() const
        {
            AutoLock l(Base::GetLock());
            
            #ifdef LOG4C
                Base::m_pValueLog->info("IsDone...");
            #endif
            const bool IsDone(Base::InternalIsDone());
            #ifdef LOG4C
                if( IsDone )
                    Base::m_pValueLog->info("...IsDone = true");
                else
                    Base::m_pValueLog->info("...IsDone = false");
            #endif
            return IsDone;
        }
    };

}

#endif
