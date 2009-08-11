//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/IEnumerationT.h,v 1.12.2.2 2009/03/27 19:00:12 hartmut_nebelung Exp $
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
\brief    Definition of interface IEnumerationT
\ingroup GenApi_PublicInterface
*/

#ifndef GENAPI_IENUMERATIONT_H
#define GENAPI_IENUMERATIONT_H

#include "GenApiDll.h"
#include "Types.h"
#include "Pointer.h"
#include "IEnumeration.h"
#include <vector>

namespace GenApi
{
    //*************************************************************
    // Enumeration template interface 
    //*************************************************************

    /**
    \brief Interface for enumeration properties
    \ingroup GenApi_PublicInterface
    */
    template< typename EnumT >
    interface IEnumerationT  : public IEnumeration 
    {
        //! Set node value
        virtual void SetValue(EnumT Value, bool Verify = true) = 0;

        //! Set node value
        virtual IEnumeration& operator=(EnumT Value) = 0;

        //! Get node value
        virtual EnumT GetValue(bool Verify = false) = 0;

        //! Get node value
        virtual EnumT operator()() = 0;

        //! Set node value
        /*! Note : the operator= is not inherited thus the operator= versions
        from IEnumeration must be implemented again */
        virtual IEnumeration& operator=(const GenICam::gcstring& ValueStr) = 0;

    };

    //*************************************************************
    // CEnumerationTRef class
    //*************************************************************

    /**
    \internal
    \brief Reference to an IEnumerationT pointer
    \ingroup GenApi_PublicImpl
    */
    template< class EnumT >
    class CEnumerationTRef : public IEnumerationT< EnumT >, public IReference, public IEnumReference
    {
    public:
        // Constructor
        CEnumerationTRef() :
          m_Ptr(NULL)
          {}

          /*--------------------------------------------------------*/
          // IReference
          /*--------------------------------------------------------*/

          //! sets the implementation to the reference
          virtual void SetReference( IBase *ptr )
          {
              m_Ptr = dynamic_cast< IEnumeration *>( ptr );
          }

          /*--------------------------------------------------------*/
          // IEnumReference
          /*--------------------------------------------------------*/

          //! sets the number of enum vallues 
          virtual void SetNumEnums( int NumEnums )
          {
              m_EnumExists.resize(NumEnums);
              m_EnumValues.resize(NumEnums);

              std::vector<bool>::iterator ptr;
              for(ptr = m_EnumExists.begin(); ptr != m_EnumExists.end(); ptr++)
                  *ptr = false;

          }

          //! sets the Enum value corresponding to a value
          virtual void SetEnumReference( int Index, GenICam::gcstring Name)
          {
              if ( m_Ptr )
              {
                  CEnumEntryPtr ptrEnumEntry = GetEntryByName(Name);

                  if( ptrEnumEntry.IsValid() )
                  {
                      m_EnumExists[Index] = true;
                      m_EnumValues[Index] = ptrEnumEntry->GetValue();
                  }
              }
          }


          /*--------------------------------------------------------*/
          // IBase
          /*--------------------------------------------------------*/

          //! Get the access mode of the node
          virtual EAccessMode GetAccessMode() const
          {
              if(m_Ptr)
                  return m_Ptr->GetAccessMode();
              else
                  return NI;
          }

          /*--------------------------------------------------------*/
          // IValue
          /*--------------------------------------------------------*/

          //! Get the INode interface of the node
          virtual INode* GetNode() 
          {
              if(m_Ptr)
                  return m_Ptr->GetNode();
              else
                  throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
          }

          //! Get content of the node as string
          virtual GenICam::gcstring ToString(bool /*Verify = false*/)
          {
              if(m_Ptr)
                  return m_Ptr->ToString(false);
              else
                  throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
          }

          //! Set content of the node as string
          virtual void FromString(const GenICam::gcstring& ValueStr, bool Verify = true)
          {
              if(m_Ptr)
                  return m_Ptr->FromString(ValueStr, Verify);
              else
                  throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
          }

          /*--------------------------------------------------------*/
          // IEnumeration
          /*--------------------------------------------------------*/

          //! Get list of symbolic Values
          virtual void GetSymbolics(StringList_t & Symbolics)
          {
              if(m_Ptr)
                  return m_Ptr->GetSymbolics(Symbolics);
              else
                  throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
          }

          //! Get list of entry nodes
          virtual void GetEntries(NodeList_t & Entries)
          {
              if(m_Ptr)
                  return m_Ptr->GetEntries(Entries);
              else
                  throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
          }

          //! Set node value
          virtual IEnumeration& operator=(const GenICam::gcstring& ValueStr)
          {
              if(m_Ptr)
                  return m_Ptr->operator=(ValueStr);
              else
                  throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
          }

          //! Set node value
          virtual IEnumeration& operator=(const char *pValueStr)
          {
              if(m_Ptr)
                  return m_Ptr->operator=(pValueStr);
              else
                  throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
          }

          //! Set node value
          virtual void SetIntValue(int64_t Value, bool Verify = true)
          {
              if(m_Ptr)
                  m_Ptr->SetIntValue(Value, Verify);
              else
                  throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
          }

          //! Get node value
          virtual GenICam::gcstring operator*()
          {
              if(m_Ptr)
                  return m_Ptr->operator*();
              else
                  throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
          }

          //! Get node value
          virtual int64_t GetIntValue(bool Verify = false)
          {
              if(m_Ptr)
                  return m_Ptr->GetIntValue(Verify);
              else
                  throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
          }

          //! Get an entry node by name
          virtual IEnumEntry *GetEntryByName(const GenICam::gcstring& EntryName)
          {
              if(m_Ptr)
                  return m_Ptr->GetEntryByName(EntryName);
              else
                  throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
          }


          /*--------------------------------------------------------*/
          // IEnumerationT
          /*--------------------------------------------------------*/

          //! Set node value
          virtual void SetValue(EnumT Value, bool Verify = true)
          {
              if(m_Ptr)
              {
                  if( m_EnumExists[Value] )
                  {
                      int64_t EnumValue = m_EnumValues[Value];
                      m_Ptr->SetIntValue(EnumValue, Verify);
                  }
                  else
                      throw ACCESS_EXCEPTION("EnumEntry %d not present", Value);
              }
              else
                  throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
          }

          //! Set node value
          virtual IEnumeration& operator=(EnumT Value)
          {
              SetValue(Value);
              return *this;
          }

          //! Get node value
          virtual EnumT GetValue(bool Verify = false)
          {
              if(m_Ptr)
              {
                  int64_t IntValue = m_Ptr->GetIntValue(Verify);
                  std::vector<int64_t>::iterator ptr;
                  for ( unsigned int idx = 0; idx < m_EnumValues.size(); ++idx )
                  {
                      if( m_EnumExists[idx] &&  m_EnumValues[idx] == IntValue)
                          return (EnumT) idx;
                  }
#ifdef _WIN32
                  throw ACCESS_EXCEPTION("Unknown IntValue %I64d", IntValue);
#else
                  throw ACCESS_EXCEPTION("Unknown IntValue %lld", IntValue);
#endif
              }
              else
                  throw ACCESS_EXCEPTION("Feature not present (reference not valid)");
          }

          //! Get node value
          virtual EnumT operator()()
          {
              return GetValue();
          }

    protected:
        //! Pointer to the implementation the reference references to
        IEnumeration *m_Ptr;

        //! The values for enums with a given index
        std::vector<int64_t > m_EnumValues;

        //! Inidcates it an enum with a given index exists
        std::vector<bool > m_EnumExists;

    };


}

#endif // ifndef GENAPI_IENUMERATIONT_H
