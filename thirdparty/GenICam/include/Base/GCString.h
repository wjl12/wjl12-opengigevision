/****************************************************************************
 (c) 2005 by STEMMER IMAGING

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


****************************************************************************/

 /// \file
 /// \brief    Portable string implementation
 /// \version  $Revision: 1.13.2.2 $
 /// \date     $Date: 2009/01/30 17:04:10 $
 /// \ingroup Base_PublicUtilities


#ifndef GENICAM_GCSTRING_H
#define GENICAM_GCSTRING_H

#include <string>

#include <iostream>
#include "GCTypes.h"

#pragma pack(push, 8)

namespace GenICam
{
    /**
    \brief A string class which is a clone of std::string
    \ingroup Base_PublicUtilities
    */
  class GCBASE_API gcstring
  {
  // Ctor / Dtor
  // -------------------------------------------------------------------------
  public:
                        gcstring              ( const char *pc = 0 );
                        gcstring              ( size_t count, char ch );
                        gcstring              ( const gcstring &str );
    virtual             ~gcstring             ( void );

  // Data access
  // -------------------------------------------------------------------------
  public:
    virtual gcstring &  append                ( const gcstring &str );
    virtual gcstring &  append                ( size_t count, char ch );
    virtual gcstring &  assign                ( const gcstring &str );
    virtual gcstring &  assign                ( size_t count, char ch );
    virtual int         compare               ( const gcstring &str )   const;
    virtual const char *c_str                 ( void )                  const;
    virtual bool        empty                 ( void )                  const;
      virtual size_t      find                  ( char ch, size_t offset = 0 ) const;
      virtual size_t      find                  ( const gcstring &str, size_t offset = 0 ) const;
      virtual size_t      find                  ( const gcstring &str, size_t offset, size_t count ) const;
      virtual size_t      find                  ( const char* pc, size_t offset = 0) const;
      virtual size_t      find                  ( const char* pc, size_t offset, size_t count ) const;
    virtual size_t      length                ( void )                  const;
    virtual size_t      size                  ( void )                  const;
    virtual    gcstring    substr                ( size_t offset = 0, size_t count = npos ) const;


  // Operators
  // -------------------------------------------------------------------------
  public:
    bool                operator !=           ( const gcstring &str )   const;
    bool                operator !=           ( const char *pc )        const;
    gcstring &          operator +=           ( const gcstring &str );
//    gcstring            operator +=           ( const gcstring &str )   const;
    gcstring &          operator +=           ( char ch );
//    gcstring            operator +=           ( char ch )               const;
    gcstring &          operator =            ( const gcstring &str );
    bool                operator ==           ( const gcstring &str )   const;
    bool                operator ==           ( const char *pc )        const;
    bool                operator <            ( const gcstring &str )   const;
    bool                operator >            ( const gcstring &str )   const;
    // to be removed?
                        operator const char * ( void )                  const;
    void                operator delete       ( void *pWhere );
    void                operator delete       ( void *pWhere, void *pNewWhere );
    void *              operator new          ( size_t uiSize );
    void *              operator new          ( size_t uiSize, void *pWhere );
    GCBASE_API
    friend gcstring     operator +            ( const gcstring &left, const gcstring &right );
    GCBASE_API
    friend gcstring     operator +            ( const gcstring &left, const char *right );
    GCBASE_API
    friend gcstring     operator +            ( const char *left, const gcstring &right );

  // Member
  // -------------------------------------------------------------------------
  private:
    std::string *       _ps;

  // Constants
  // -------------------------------------------------------------------------
  public:
    static const size_t npos;
  };
  
  GCBASE_API
  std::istream &  getline ( std::istream& is, GenICam::gcstring& str );
  GCBASE_API
  std::istream &  getline ( std::istream& is, GenICam::gcstring& str, char delim );
}

//! STL operator out
//! \ingroup Base_PublicUtilities
inline std::ostream & operator <<(std::ostream &ostr, const GenICam::gcstring &str) { return ostr << str.c_str(); }

//! STL operator in
//! \ingroup Base_PublicUtilities
inline std::istream & operator >>(std::istream &istr, GenICam::gcstring &str) 
{
  std::string tmp;
  istr >> tmp;
  str = tmp.c_str();
  return istr;
}

#pragma pack(pop)

#endif // GENICAM_GCSTRING_H
