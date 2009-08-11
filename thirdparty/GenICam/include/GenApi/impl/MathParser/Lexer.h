//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/MathParser/Lexer.h,v 1.9 2007/08/16 13:56:49 hartmut_nebelung Exp $
//
//  License: Derived from Kirill Zaborski's MathParser library (http://kirya.narod.ru/mathparser.html ). 
//  This library comes under LGPL license (see http://www.gnu.org/licenses/lgpl.html). 
//  Kirill's implementation is a C++ port of the CCalc library from Walery Studennikov (http://www.sama.ru/~despair/ccalc/)
//  which also comes under the LGPL.
//-----------------------------------------------------------------------------
/*!
\file
\brief    Definition of CLexer
*/

#ifndef _LEXER_H_
#define _LEXER_H_

#if defined (_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#include <cstdlib>

#include <GenApi/Types.h>
#include <GenApi/impl/Compatibility.h>
#include "SymTable.h"



typedef unsigned char uchar;

typedef enum {
    CH_LETTER = 0x01, CH_DIGIT = 0x02, CH_SEPARAT = 0x04,
    CH_SYMBOL = 0x08, CH_QUOTE = 0x10,
    CH_UNKNOWN= 0x7E, CH_FINAL = 0x7F
} hqCharType;

typedef enum {
    TOK_ERROR, TOK_NONE, TOK_FINAL, TOK_INT, TOK_FLOAT, TOK_SYMBOL,
    TOK_NAME, TOK_STRING
} hqTokenType;

class CLexer{
    char       *m_pSS;
    char       *SS;
public:
    // input params
    int        cssn;    // Comment Start Symbol Number. -1 if none
    char    *ComEnd;    // End of comment
    CSymTable *SymTable;
    hqCharType *CharTypeTable;

    // output params
    char       *Name;
    size_t        NameLen;
    double    ExtValue;
    int        IntValue;
    hqTokenType PrevTokenType;
    hqCharType    CharType;
    int        NoIntegers;
    int SetParseString(const char *str );
    hqTokenType GetNextToken();
    CLexer();
    ~CLexer();
    char* GetCurrentPos();
};

/* Misc */

void InitCharTypeTable( hqCharType *CharTypeTable, int CharTypes );

#endif //_LEXER_H_
