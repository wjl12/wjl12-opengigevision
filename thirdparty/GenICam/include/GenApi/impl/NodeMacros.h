//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/NodeMacros.h,v 1.15 2007/11/13 19:41:05 hartmut_nebelung Exp $
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
\brief    Defintion of macros for property conversion map
*/

#ifndef GENAPI_NODEMACROS_H
#define GENAPI_NODEMACROS_H

//*************************************************************
// Macros forming the CONVERSION_MAP
//*************************************************************

typedef enum
{
    In,
    Out
} Direction_t;


//! Starts a conversion map; BASE 
#define BEGIN_CONVERT_MAP \
virtual bool SetProperty(const GenICam::gcstring& PropertyName, const GenICam::gcstring& ValueStr) \
    { \
        return SetProperty(PropertyName, ValueStr, ""); \
    } \
virtual bool SetProperty(const GenICam::gcstring& PropertyName, const GenICam::gcstring& ValueStr, const GenICam::gcstring& AttributeStr) \
    { \
        bool PropertyAlreadyPresent=false; \
        GenICam::gcstring_vector::iterator ptr; \
        for(ptr=m_PropertyNames.begin(); ptr!=m_PropertyNames.end(); ptr++) \
            if(*ptr == PropertyName) \
                PropertyAlreadyPresent = true; \
        if(!PropertyAlreadyPresent) \
            m_PropertyNames.push_back(PropertyName); \
        GenICam::gcstring _ValueStr=ValueStr; \
        GenICam::gcstring _AttributeStr = AttributeStr; \
        return AccessProperty( PropertyName, _ValueStr, _AttributeStr, In ); \
    } \
virtual bool AccessProperty(const GenICam::gcstring& PropertyName, GenICam::gcstring& ValueStr, GenICam::gcstring& AttributeStr, Direction_t Direction) \
  { \
    if(    Direction == Out ) \
    { \
        ValueStr = ""; \
        AttributeStr = ""; \
    } \
    bool dummy=false; \
    if(dummy) \
        {return false;}


//! Chains the call to another class
#define CHAIN_CONVERT_MAP(TARGET) \
  else if(TARGET::AccessProperty(PropertyName, ValueStr, AttributeStr, Direction)) \
    return true;

//! Entry in the conversion map for simple values
#define CONVERT_ENTRY(Name, Member) \
    else if (Direction == In && PropertyName == #Name) \
    { \
        if( !String2Value(ValueStr, &Member)) \
            throw PROPERTY_EXCEPTION("%s : property '%s' : cannot convert value '%s'", m_Name.c_str(), PropertyName.c_str(), ValueStr.c_str() ); \
        return true; \
    } \
    else if (Direction == Out && PropertyName == #Name) \
    { \
        Value2String(Member, ValueStr); \
        return true; \
    }

#define CONVERT_STRING_ENTRY(Name, Member) \
    else if (Direction == In && PropertyName == #Name) \
    { \
        Member = ValueStr; \
        return true; \
    } \
    else if (Direction == Out && PropertyName == #Name) \
    { \
        ValueStr = Member; \
        return true; \
    }

//! Entry in the conversion map for simple values
#define CONVERT_LIST_ENTRY(Name, Member, _Type) \
    else if (Direction == In && PropertyName == #Name) \
    { \
        _Type listElem;\
        if( !String2Value(ValueStr, &listElem)) \
            throw PROPERTY_EXCEPTION("%s : property '%s' : cannot convert value '%s'", m_Name.c_str(), PropertyName.c_str(), ValueStr.c_str() ); \
        Member.push_back(listElem);\
        return true; \
    } \
    else if (Direction == Out && PropertyName == #Name) \
   {  Values2String( Member, ValueStr ); return true; }
#if 0
        std::list<_Type>::iterator ptr; \
        for(ptr=Member.begin(); ptr!=Member.end(); ptr++ ) \
        { \
            GenICam::gcstring Buffer; \
            Value2String(*ptr, Buffer); \
            ValueStr += Buffer + "\t"; \
        } \
        if(ValueStr.size() > 1) \
            ValueStr = ValueStr.substr(0, ValueStr.size()-1); \
        return true; \
    }
#endif

//! Entry in the conversion map for invalidators
#define CONVERT_INVALIDATOR( _Name ) \
    else if (Direction == In && PropertyName == #_Name) \
    { \
        INode *pNode = m_pNodeMap->GetNode( ValueStr ); \
        if (! pNode) \
            throw PROPERTY_EXCEPTION("%s : property '%s' : node '%s' does not exist", dynamic_cast<INode*>(this)->GetName().c_str(), PropertyName.c_str(), ValueStr.c_str() ); \
        m_Invalidators.push_back( pNode ); \
        return true; \
    } \
    else if (Direction == Out && PropertyName == #_Name) \
    { \
        node_vector::iterator ptr; \
        for(ptr=m_Invalidators.begin(); ptr!=m_Invalidators.end(); ptr++ ) \
        { \
            ValueStr += (*ptr)->GetName() + "\t"; \
        } \
        if(ValueStr.size() > 1) \
            ValueStr = ValueStr.substr(0, ValueStr.size()-1); \
        return true; \
    }

/**
\brief Entry in the conversion map for node refs
*/
#define CONVERT_NODE_REFERENCE(_Name, Member, Interface) \
    else if (Direction == In && PropertyName == #_Name) \
    { \
        INode * const pNode = m_pNodeMap->GetNode(ValueStr); \
        if(!pNode) \
            throw PROPERTY_EXCEPTION("%s : property '%s' : node '%s' does not exist", dynamic_cast<INode*>(this)->GetName().c_str(), PropertyName.c_str(), ValueStr.c_str() ); \
        Member = dynamic_cast<Interface*>(pNode); \
        if(!Member) \
            throw PROPERTY_EXCEPTION("%s : property '%s' : node '%s' has no interface '" #Interface "'", dynamic_cast<INode*>(this)->GetName().c_str(), PropertyName.c_str(), ValueStr.c_str() ); \
        if (dynamic_cast<INode*>(this) == pNode) \
        throw PROPERTY_EXCEPTION("%s : property '%s' : self reference ", dynamic_cast<INode*>(this)->GetName().c_str(), PropertyName.c_str() ); \
        AddChild(pNode ); \
        return true; \
    } \
    else if (Direction == Out && PropertyName == #_Name) \
    { \
        INode *pNode = dynamic_cast<INode*>(Member); \
        if(pNode) \
            ValueStr = pNode->GetName(); \
        return true; \
    }

/**
\brief Entry in the conversion map for node refs
*/
#define CONVERT_NODELIST_REFERENCE_AUX(_Name, Member, Interface, AuxMember, AuxType ) \
    else if (Direction == In && PropertyName == #_Name) \
    { \
        INode * const pNode = m_pNodeMap->GetNode(ValueStr); \
        if(!pNode) \
            throw PROPERTY_EXCEPTION("%s : property '%s' : node '%s' does not exist", dynamic_cast<INode*>(this)->GetName().c_str(), PropertyName.c_str(), ValueStr.c_str() ); \
        Interface * const pListElem = dynamic_cast<Interface*>(pNode); \
        if(!pListElem) \
            throw PROPERTY_EXCEPTION("%s : property list element '%s' : node '%s' has no interface '" #Interface "'", dynamic_cast<INode*>(this)->GetName().c_str(), PropertyName.c_str(), ValueStr.c_str() ); \
        Member.push_back(pListElem);\
        dynamic_cast<CNodeImpl*>(this)->AddChild(pNode); \
        AuxType AuxValue; \
        if( !String2Value(AttributeStr, &AuxValue)) \
            throw PROPERTY_EXCEPTION("%s : property '%s' : cannot convert attribute value '%s'", m_Name.c_str(), PropertyName.c_str(), AttributeStr.c_str() ); \
        AuxMember.push_back(AuxValue);\
        return true; \
    } \
    else if (Direction == Out && PropertyName == #_Name) \
    { \
        IntegerList_t::iterator ptrValue; \
        Int64List_t::iterator ptrAttribute; \
        for(ptrValue=Member.begin(), ptrAttribute=AuxMember.begin(); \
            ptrValue!=Member.end();  \
            ptrValue++, ptrAttribute++ ) \
        { \
            ValueStr += (*ptrValue)->GetNode()->GetName() + "\t"; \
            GenICam::gcstring Buffer; \
            Value2String(*ptrAttribute, Buffer); \
            AttributeStr += Buffer + "\t"; \
        } \
        if(ValueStr.size() > 1) \
        { \
            ValueStr = ValueStr.substr(0, ValueStr.size()-1); \
            AttributeStr = AttributeStr.substr(0, AttributeStr.size()-1); \
        } \
        return true; \
    }


/**
\brief Entry in the conversion map for node refs
*/
#define CONVERT_NODELIST_REFERENCE(_Name, Member, Interface, ContainerType ) \
    else if (Direction == In && PropertyName == #_Name) \
    { \
        INode * const pNode = m_pNodeMap->GetNode(ValueStr); \
        if(!pNode) \
            throw PROPERTY_EXCEPTION("%s : property '%s' : node '%s' does not exist", dynamic_cast<INode*>(this)->GetName().c_str(), PropertyName.c_str(), ValueStr.c_str() ); \
        Interface * const pListElem = dynamic_cast<Interface*>(pNode); \
        if(!pListElem) \
            throw PROPERTY_EXCEPTION("%s : property list element '%s' : node '%s' has no interface '" #Interface "'", dynamic_cast<INode*>(this)->GetName().c_str(), PropertyName.c_str(), ValueStr.c_str() ); \
        Member.push_back(pListElem);\
        dynamic_cast<CNodeImpl *>(this)->AddChild(pNode); \
        return true; \
    } \
    else if (Direction == Out && PropertyName == #_Name) \
    { \
        ContainerType::iterator ptr; \
        for(ptr=Member.begin(); ptr!=Member.end(); ptr++ ) \
            ValueStr += (*ptr)->GetNode()->GetName() + "\t"; \
        if(ValueStr.size() > 1) \
            ValueStr = ValueStr.substr(0, ValueStr.size()-1); \
        return true; \
    }


/**
\brief Entry in the conversion map for node refs that are not children
*/
#define CONVERT_NODELIST_REFERENCE2(_Name, Member, Interface) \
    else if (Direction == In && PropertyName == #_Name) \
    { \
        INode * const pNode = m_pNodeMap->GetNode(ValueStr); \
        if(!pNode) \
            throw PROPERTY_EXCEPTION("%s : property '%s' : node '%s' does not exist", dynamic_cast<INode*>(this)->GetName().c_str(), PropertyName.c_str(), ValueStr.c_str() ); \
        Interface * const pListElem = dynamic_cast<Interface*>(pNode); \
        if(!pListElem) \
            throw PROPERTY_EXCEPTION("%s : property list element '%s' : node '%s' has no interface '" #Interface "'", dynamic_cast<INode*>(this)->GetName().c_str(), PropertyName.c_str(), ValueStr.c_str() ); \
        Member.push_back(pListElem);\
        return true; \
    } \
    else if (Direction == Out && PropertyName == #_Name) \
    { \
        FeatureList_t::iterator ptr; \
        for(ptr=Member.begin(); ptr!=Member.end(); ptr++ ) \
        { \
            ValueStr += (*ptr)->GetNode()->GetName() + "\t"; \
        } \
        if(ValueStr.size() > 1) \
            ValueStr = ValueStr.substr(0, ValueStr.size()-1); \
        return true; \
    }

/**
\brief Entry in the conversion map for refs
*/
#define CONVERT_REFERENCE(_Name, Member, Interface) \
    else if (Direction == In && PropertyName == #_Name) \
    { \
        INode * const pNode = dynamic_cast<INode*>(this)->GetNode(ValueStr); \
        if(!pNode) \
            throw PROPERTY_EXCEPTION("%s : property '%s' : node '%s' does not exist", m_Name.c_str(), PropertyName.c_str(), ValueStr.c_str() ); \
        Member = dynamic_cast<Interface*>(pNode); \
        return true; \
    } \
    else if (Direction == Out && PropertyName == #_Name) \
    { \
        INode * const pNode = dynamic_cast<INode*>(Member); \
        ValueStr = pNode->GetName(); \
        return true; \
    } 

//! Entry in the conversion map for node refs
#define CONVERT_ANONYMOUS_NODE_REFERENCE(_Name) \
    else if (Direction == In && PropertyName == #_Name) \
    { \
        INode * const pNode = m_pNodeMap->GetNode(ValueStr); \
        if(!pNode) \
            throw PROPERTY_EXCEPTION("%s : property '%s' : node '%s' does not exist", m_Name.c_str(), PropertyName.c_str(), ValueStr.c_str() ); \
        dynamic_cast<CNodeImpl*>(this)->AddChild(pNode); \
        return true; \
    } \
    else if (Direction == Out && PropertyName == #_Name) \
    { \
        INode * const pNode = dynamic_cast<INode*>(Member); \
        ValueStr = pNode->GetName(); \
        return true; \
    } 

//! Entry in the conversion map for GenApi defined enumeration types
#define CONVERT_ENUM_ENTRY(_Name, Member, _EnumDelegate) \
    else if (Direction == In && PropertyName == GenICam::gcstring(#_Name)) \
    { \
        if( !_EnumDelegate::FromString(ValueStr, &(Member))) \
            throw PROPERTY_EXCEPTION("%s : property '%s' : cannot convert value '%s'", m_Name.c_str(), PropertyName.c_str(), ValueStr.c_str() ); \
        return true; \
    } \
    else if (Direction == Out && PropertyName == GenICam::gcstring(#_Name)) \
    { \
        ValueStr = _EnumDelegate::ToString(Member); \
        return true; \
    }

//! Entry in the conversion map for user defined enumeration types
#define ADD_ENUM_ENTRY(_Name, _EnumDelegate) \
    else if (Direction == In && PropertyName == #_Name) \
    { \
        INode * const pNode = m_pNodeMap->GetNode(ValueStr); \
        if(!pNode) \
            throw PROPERTY_EXCEPTION("%s : property '%s' : node '%s' does not exist", dynamic_cast<INode*>(this)->GetName().c_str(), PropertyName.c_str(), ValueStr.c_str() ); \
        IEnumEntry * const pEntry = dynamic_cast<IEnumEntry *>(pNode); \
        if(!pEntry) \
            throw PROPERTY_EXCEPTION("%s : property enum entry '%s' : node '%s' is not of class IEnumEntry ", dynamic_cast<INode*>(this)->GetName().c_str(), PropertyName.c_str(), ValueStr.c_str() ); \
        _EnumDelegate->AddEntry(pNode); \
        return true; \
    } \
    else if (Direction == Out && PropertyName == #_Name) \
    { \
        NodeList_t Entries; \
        _EnumDelegate->GetEntries(Entries); \
        NodeList_t::iterator ptr; \
        for(ptr=Entries.begin(); ptr!=Entries.end(); ptr++ ) \
            ValueStr += (*ptr)->GetName() + "\t"; \
        if(ValueStr.size() > 1) \
            ValueStr = ValueStr.substr(0, ValueStr.size()-1); \
        return true; \
    } \

//! Entry in the conversion map for user defined variables of the swiss knife
#define ADD_VARIABLE_ENTRY(_Name, _VariableDelegate) \
    else if (Direction == In && PropertyName == #_Name) \
    { \
        _VariableDelegate->AddEntry(ValueStr, AttributeStr, m_Name, m_pNodeMap); \
        return true; \
    } \
    else if (Direction == Out && PropertyName == #_Name) \
    { \
        _VariableDelegate->GetVariablesAndAttributes(ValueStr, AttributeStr); \
        return true; \
    }

//! Terminates the conversion map
#define END_CONVERT_MAP \
    else \
        return false; \
};


#endif  // GENAPI_NODEMACROS_H
