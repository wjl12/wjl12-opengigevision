//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/MathParser/SymTable.h,v 1.7 2007/08/16 13:56:50 hartmut_nebelung Exp $
//
//  License: Derived from Kirill Zaborski's MathParser library (http://kirya.narod.ru/mathparser.html ). 
//  This library comes under LGPL license (see http://www.gnu.org/licenses/lgpl.html). 
//  Kirill's implementation is a C++ port of the CCalc library from Walery Studennikov (http://www.sama.ru/~despair/ccalc/)
//  which also comes under the LGPL.
//-----------------------------------------------------------------------------
/*!
\file
\brief    Implementation of CSymTable
*/
#if !defined(AFX_SYMTABLE_H__F8B4AD7F_07F9_4AB2_B206_04549837C7ED__INCLUDED_)
#define AFX_SYMTABLE_H__F8B4AD7F_07F9_4AB2_B206_04549837C7ED__INCLUDED_

#if defined (_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

class CSymTable  
{
public:
    typedef struct {
        char Sym[4];
        char Len;
        char Index;
        char More;
    } SymbolRec;
private:
    SymbolRec* table[256];
public:
    void PrepareSymTable( const char *symbols );
    int FindSymbol( const char *str, int *nchars );
    CSymTable();
    virtual ~CSymTable();

};

#endif // !defined(AFX_SYMTABLE_H__F8B4AD7F_07F9_4AB2_B206_04549837C7ED__INCLUDED_)
