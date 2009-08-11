//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Margret Albrecht
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/MathParser/StrMap.h,v 1.7.2.2 2008/12/12 18:23:03 hartmut_nebelung Exp $
//
//  License: Derived from Kirill Zaborski's MathParser library (http://kirya.narod.ru/mathparser.html ). 
//  This library comes under LGPL license (see http://www.gnu.org/licenses/lgpl.html). 
//  Kirill's implementation is a C++ port of the CCalc library from Walery Studennikov (http://www.sama.ru/~despair/ccalc/)
//  which also comes under the LGPL.
//-----------------------------------------------------------------------------
/*!
\file
\brief    Definition of CStrMap
*/
#ifndef _STRMAP_H_
#define _STRMAP_H_


#if defined (_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#include "MathParserDll.h"

/**
* \ingroup internal_impl
*
* \brief Map for variables in swissknife expression
*/
class MATHPARSERDLL_API CStrMap
{
  int   FCount, FCapacity;
  int   FExtraLen, FRecordLen;
  int   FDoDuplicate;
  char *FList;
public:
  CStrMap(int extrabytes=sizeof(double), int dup=0);
  void CreateFromChain( int extrabytes, const char *strchain, const void *data );
  ~CStrMap(void);
  void AddString(const char *str, void *data);
  void AddStrLen(const char *str, size_t len, const void *data);
  void ShrinkMem(void);
  void Trim(int NewCount);
  void TrimClear(int NewCount);
  void SetCapacity(int NewCapacity);
  int IndexOf(const char *str, const void **data);
  int LenIndexOf(const char *str, size_t len, const void **data);
  int Replace(const char *str,void *data);
  int LenReplace(const char *str, size_t len, void *data);
  const char * GetString(int index, size_t *len, void **data);
  void FillFromChain(const char *strchain, const void *data);
};

#endif //_STRMAP_H_
