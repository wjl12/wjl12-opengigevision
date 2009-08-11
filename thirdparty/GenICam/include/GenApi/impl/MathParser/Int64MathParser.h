//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/MathParser/Int64MathParser.h,v 1.8.2.1 2009/03/27 09:59:48 hartmut_nebelung Exp $
//
//  License: Derived from Kirill Zaborski's MathParser library (http://kirya.narod.ru/mathparser.html ). 
//  This library comes under LGPL license (see http://www.gnu.org/licenses/lgpl.html). 
//  Kirill's implementation is a C++ port of the CCalc library from Walery Studennikov (http://www.sama.ru/~despair/ccalc/)
//  which also comes under the LGPL.
//-----------------------------------------------------------------------------
/*!
\file
\brief    Definition of CInt64MathParser

* CInt64MathParser is derived from Kirill Zaborski's MathParser library (http://kirya.narod.ru/mathparser.html ). 
* This library is under LGPL license. Kirill's implementation seems to be a C++ port of the CCalc library
* from Walery Studennikov (http://www.sama.ru/~despair/ccalc/).
*/

#ifndef _INT64MATHPARSER_H_
#define _INT64MATHPARSER_H_

#if defined (_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#include <GenApi/Types.h>
#include <GenApi/GenApiDll.h>

#include "MathParserDll.h"
#include "StrMap.h"
#include "Int64Lexer.h"

#include <vector>

#ifdef _MSC_VER // *JS*
#pragma warning(push)
#pragma warning(disable: 4251) // class 'xxx' needs to have dll-interface to be used by clients of class 'yyy'
#endif

#define ORIGINAL_STACK_SIZE 64

extern const double DblErR;


/**
* \ingroup internal_impl
*
* \brief Parser and evaluator for CIntSwissKnife
* 
*/

class MATHPARSERDLL_API CInt64MathParser{
  typedef int64_t (*OneArgFunc) ( int64_t arg );
  typedef char* (*MultiArgFunc) ( int paramcnt, int64_t *args,
    CStrMap *strparams, int64_t *result );
  typedef int (*PrmSrchFunc) ( const char *str, size_t len, int64_t *value,
    void *param );
  typedef enum {
    // Binary
    OP_SHL, OP_SHR, OP_POW,
    OP_LOGIC_NEQ, OP_LOGIC_GEQ, OP_LOGIC_LEQ,
    OP_LOGIC_AND, OP_LOGIC_OR, // Logical
    OP_COMSTART, OP_ASSIGN, // For internal needs
    OP_OBR, // Special
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD, OP_UNK, // Arithmetic
    OP_XOR, OP_NOT, OP_AND, OP_OR, // Bitwise
    OP_EQU, OP_GREATER, OP_LESS,
    OP_LOGIC, OP_LOGIC_SEP, OP_CBR, OP_COMMA, // Logical
    OP_FORMULAEND, // For script
    OP_FUNC_ONEARG, OP_FUNC_MULTIARG // Special
  } OperType_t;
  static const char OpPriorities[OP_FUNC_MULTIARG+1];
  typedef struct {
    OperType_t  OperType;
        const void  *Func;
        char    PrevValTop;
        CStrMap *StrParams;
    } Operation;
    static const Operation BrOp;
    static const Operation NegOp;
	std::vector<Operation> OpStack;
	std::vector<int64_t> ValStack;
    int    OpTop, ValTop;
    int    ObrDist;
    CInt64Lexer Lexer;
    int    script;
    CStrMap *VarParams;
    static CStrMap IntFunctions;
    static CSymTable MathSymTable;
    static char errbuf[256];
    static hqCharType MathCharTypeTable[256];
    static int initializations_performed;
    const char * ParseScript(int64_t *result);
    const char * ParseFormula(int64_t *result);
    const char * PrepareFormula();
    const char * Calc();
    const char * CalcToObr();
public:
    CStrMap *Parameters;
    CStrMap *ExtFunctions;
    PrmSrchFunc MoreParams;
    void  *ParamFuncParam;
    CInt64MathParser( const char *MoreLetters=NULL );
    const char * Parse( const char *Formula, int64_t *result);
    ~CInt64MathParser();
};

#ifdef _MSC_VER // *JS*
#pragma warning(pop)
#endif


#endif // _INT64MATHPARSER_H_
