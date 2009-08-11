//-----------------------------------------------------------------------------
//  (c) 2006 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/Types.h,v 1.15 2007/08/14 13:21:01 hartmut_nebelung Exp $
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
\brief Common types used in the public GenApi interface.
\ingroup GenApi_PublicUtilities
*/

#ifndef GENAPI_TYPES_H
#define GENAPI_TYPES_H

#ifdef _WIN32
#include <objbase.h> // interface
#else
#define interface struct
#endif

#include <Base/GCTypes.h>
#include <Base/GCStringVector.h>

namespace GenApi
{

    //! signed or unsigned integers
    //! \ingroup GenApi_PublicUtilities
    typedef enum 
    {
        Signed,     //!< Integer is signed
        Unsigned,   //!< Integer is unsigned
        _UndefinedSign  //!< Object is not yetinitialized
    } ESign;

    //! access mode of a node
    //! \ingroup GenApi_PublicUtilities
    typedef enum 
    {
        NI,        //!< Not implemented
        NA,        //!< Not available
        WO,        //!< Write Only
        RO,        //!< Read Only
        RW,        //!< Read and Write
        _UndefinedAccesMode //!< Object is not yetinitialized
    } EAccessMode; 

    //! recommended visibility of a node
    //! \ingroup GenApi_PublicUtilities
    typedef enum 
    {
        Beginner = 0,        //!< Always visible
        Expert = 1,        //!< Visible for experts or Gurus
        Guru = 2,        //!< Visible for Gurus
        Invisible = 3,    //!< Not Visible
        _UndefinedVisibility  = 99 //!< Object is not yetinitialized
    } EVisibility;

  //! caching mode of a register
  //! \ingroup GenApi_PublicUtilities
  typedef enum
  {
    NoCache,              //!< Do not use cache
    WriteThrough,         //!< Write to cache and register
    WriteAround,          //!< Write to register, write to cache on read
    _UndefinedCachingMode //!< Not yet initialized
  } ECachingMode;

    //! recommended representation of a node value
    //! \ingroup GenApi_PublicUtilities
    typedef enum 
    {
        Linear,            //!< Slider with linear behaviour
        Logarithmic,    //!< Slider with logarithmic behaviour
        Boolean,        //!< Checkbox
        PureNumber,        //!< Decimal number in an edit control
        HexNumber,        //!< Hex number in an edit control
        _UndefindedRepresentation
    } ERepresentation;

    //! Endianess of a value in a register
    //! \ingroup GenApi_PublicUtilities
    typedef enum 
    {
        BigEndian,     //!< Register is big endian
        LittleEndian,   //!< Register is little endian
        _UndefinedEndian  //!< Object is not yetinitialized
    } EEndianess;


    //! Defines if a node name is standard or custom
    //! \ingroup GenApi_PublicUtilities
    typedef enum 
    {
        Custom,            //!< name resides in custom namespace
        Standard,        //!< name resides in one of the standard namespaces
        _UndefinedNameSpace  //!< Object is not yetinitialized
    } ENameSpace;


    //! Defines from which standard namespace a node name comes from 
    //! \ingroup GenApi_PublicUtilities
    typedef enum 
    {
        None,            //!< name resides in custom namespace
        GEV,            //!< name resides in GigE Vision namespace
        IIDC,            //!< name resides in 1394 IIDC namespace
        CL,            //!< name resides in camera link namespace
        _UndefinedStandardNameSpace  //!< Object is not yetinitialized
    } EStandardNameSpace;


    //! Defines the chices of a Yes/No alternaitve
    //! \ingroup GenApi_PublicUtilities
    typedef enum 
    {
        Yes,            //!< yes
        No,                //!< no
        _UndefinedEYesNo  //!< Object is not yetinitialized
    } EYesNo;

    //! A list of strings
    //! \ingroup GenApi_PublicImpl
    typedef GenICam::gcstring_vector StringList_t;


    //! typedef for fomula type
    //! \ingroup GenApi_PublicImpl
    typedef enum ESlope
    {
        Increasing, //!> strictly monotonous increasing
        Decreasing,    //!> strictly monotonous decreasing
        Varying,    //!> slope changes, e.g. at run-time
        Automatic,    //!> slope is determined automatically by probing the function
        _UndefinedESlope  //!< Object is not yetinitialized
    };

}

#endif // ifndef GENAPI_TYPES_H
