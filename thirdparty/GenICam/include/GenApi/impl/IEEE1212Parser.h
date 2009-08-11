//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Hartmut Nebelung
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/IEEE1212Parser.h,v 1.11 2007/11/13 19:41:05 hartmut_nebelung Exp $
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
\brief    Definition of CIEEE1212Parser
*/


#ifndef GENAPI_IEEE1212PARSER_H
#define GENAPI_IEEE1212PARSER_H

#include <map>
#include "../Types.h"
#include "Register.h"

#ifdef _MSC_VER // *JS*
#pragma warning(push)
#pragma warning(disable: 4251) // class 'xxx' needs to have dll-interface to be used by clients of class 'yyy'
#endif

namespace GenApi
{

    /*!
    * \ingroup internal_impl
    *
    * Parser for a IEEE1212 compatible configuration rom.
    * Allows readonly access to a single unit.
    *
    * The Unit property defines which unit is selected in
    * the configuration rom. It defaults to the value for
    * DCAM.
    *
    * Values and Textual descriptors are cached in seperate
    * maps, allowing a fast retrieval. If a change of the
    * connected unit is detected, all caches are invalidated.
    * This class is serves the CTxtkey and CIntKey classes as
    * information provider.
    */
    class GENAPI_DECL CIEEE1212ParserImpl : public CRegisterImpl
    {
    public:
        CIEEE1212ParserImpl(void);
        ~CIEEE1212ParserImpl(void);

        void FinalConstruct();

    protected:
        //-------------------------------------------------------------
        // IValue implementation
        //-------------------------------------------------------------

        //! Get value of the node as string
        virtual GenICam::gcstring InternalToString(bool Verify = false);

        //-------------------------------------------------------------
        // INodePrivate implementation
        //-------------------------------------------------------------

        //! SetInvalid
        virtual void SetInvalid(INodePrivate::ESetInvalidMode simMode);

    public:
        //-------------------------------------------------------------
        // Implementation
        //-------------------------------------------------------------
        bool GetValue( uint8_t Key, uint64_t& Value );
        bool GetDescriptor( uint8_t Key, GenICam::gcstring& Value );


        //-------------------------------------------------------------
        // Initializing
        //-------------------------------------------------------------
    public:
        //! Convert map
        BEGIN_CONVERT_MAP
            CONVERT_ENTRY( Unit, m_UnitId )
            CHAIN_CONVERT_MAP(CRegisterImpl)
        END_CONVERT_MAP

        typedef uint8_t Key_t;
    private:
        // map type for numeric values
        typedef std::map< Key_t, uint32_t > ValueMap_t;
        /// map type for textual descriptors
        typedef std::map< Key_t, GenICam::gcstring > DescriptorMap_t;

    protected:
        virtual void InternalGet(uint8_t *, int64_t );
    private:
        bool CheckAddressRange( void *pv ) const;
        bool CheckIdentity() const;

        bool ParseConfRom( Key_t );
        bool ParseDirectory( uint32_t *p, Key_t searchkey, Key_t mapkey );
        bool ParseDescriptor( uint32_t *p, Key_t key );
        bool ParseRootDirectory( uint32_t *p, Key_t key );
        bool ParseUnitDependentDirectory( uint32_t *p, Key_t key );
        bool ParseUnitDirectory( uint32_t *p, Key_t key );
        static uint64_t UnitIdentifier( const uint32_t BusInfo[] );
        uint32_t* LocateUnitDirectory( uint32_t *pInstance, Key_t key=0xD1);
        bool InsertValues( uint32_t *q, bool lookup , Key_t key );

        uint32_t m_UnitId;   ///< Unit Specifier Id, assuming uniqueness
        uint32_t m_CharSet;  ///< 0 = minimal ascii (default)
        uint32_t m_Language; ///< 0 = English (default)
        uint64_t m_Identity; ///< Unit Identifier

        /// Pointer to the Unit Directory
        uint32_t *m_pUnitDirectory;
        
        /// Map with numerical values found
        ValueMap_t m_ValueMap;
        
        /// Map with strings found
        DescriptorMap_t m_DescMap;

    };

    class GENAPI_DECL CIEEE1212Parser : public BaseT< ValueT< RegisterT< NodeT< CIEEE1212ParserImpl > > > >
    {
    };

}

#ifdef _MSC_VER // *JS*
#pragma warning(pop)
#endif

#endif // GENAPI_IEEE1212PARSER_H
