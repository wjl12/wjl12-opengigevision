//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/MaskedIntReg.h,v 1.9 2007/08/16 13:56:25 hartmut_nebelung Exp $
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
\brief    Definition of CMaskedIntReg
*/

#ifndef GENAPI_MASKEDINTREG_H
#define GENAPI_MASKEDINTREG_H

#include "IntReg.h"


namespace GenApi
{
    //*************************************************************
    // CMaskedIntReg class
    //*************************************************************

    //! IInteger implementation for a masked register
    /*! Maps an integer (signed or unsigned) to a masked register
    */
    class GENAPI_DECL CMaskedIntRegImpl : public CIntRegImpl
    {
    public:
        //! Constructor
        CMaskedIntRegImpl();


        //-------------------------------------------------------------
        // INodePrivate implementation
        //-------------------------------------------------------------


        //! Initializes the object
        virtual void FinalConstruct();

        //-------------------------------------------------------------
        // IInteger implementation
        //-------------------------------------------------------------

    protected:
        //! Set feature value
        virtual void InternalSetValue(int64_t Value, bool Verify = true);


        //! Get feature value
        virtual int64_t InternalGetValue(bool Verify = false);

    public:


        //-------------------------------------------------------------
        // Initializing
        //-------------------------------------------------------------
        //! Convert map
        BEGIN_CONVERT_MAP
            CONVERT_ENTRY(LSB, m_LSB)
            CONVERT_ENTRY(MSB, m_MSB)
            CHAIN_CONVERT_MAP(CIntRegImpl)
        END_CONVERT_MAP

    protected:
        //-------------------------------------------------------------
        // Memeber variables
        //-------------------------------------------------------------
        //! least siginificant bit of the integer with respect to the register
        uint32_t  m_LSB;

        //! most siginificant bit of the integer with respect to the register
        uint32_t  m_MSB;

        //! Mask with 1 were the integer is located; 0 otherwise
        uint64_t m_Mask;


    };

    class GENAPI_DECL CMaskedIntReg 
        : public BaseT< ValueT< NodeT < IntegerT< RegisterT<  CMaskedIntRegImpl> > > > >
    {
    };

}

#endif // GENAPI_MASKEDINTREG_H
