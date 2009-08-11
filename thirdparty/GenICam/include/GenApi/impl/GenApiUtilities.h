//-----------------------------------------------------------------------------
//  (c) 2005 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenICam
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/impl/GenApiUtilities.h,v 1.2 2006/07/18 13:31:53 rueder Exp $
//-----------------------------------------------------------------------------
/*!
\file     GenApiUtilities.h
\brief    GenICam common utilities
*/

#ifndef GENAPI_GENAPIUTILITIES_DEF_H_
#define GENAPI_GENAPIUTILITIES_DEF_H_

#include <string>
#include <Base/GCString.h>

namespace GenApi
{

  void ReplaceEnvironmentVariables(GenICam::gcstring &Buffer);

}

#endif // GENAPI_GENAPIUTILITIES_DEF_H_
