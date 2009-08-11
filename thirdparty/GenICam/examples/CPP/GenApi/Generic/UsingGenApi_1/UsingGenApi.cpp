//-----------------------------------------------------------------------------
//  (c) 2006, 2007 by Basler Vision Technologies
//  Section: Vision Components
//  Project: GenApi
//  Author:  Fritz Dierks
//  $Header: /cvs/genicam/genicam/examples/CPP/GenApi/Generic/UsingGenApi_1/UsingGenApi.cpp,v 1.9 2007/12/17 13:58:10 hartmut_nebelung Exp $
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
    \brief    Shows how to use GenApi 
*/

#if WIN32
// Required for SetDllDirectory; this function exists only with WinXK SP1 or higher
#define _WIN32_WINNT 0x0502
#include <Windows.h>
#endif 
#include <stdio.h>
#include <conio.h>
#include <string>

// Required for using GenApi
#include <GenApi/GenApi.h>
using namespace GenICam;
using namespace GenApi;

#if WIN32
int setenv( const char name[], const char value[], int replace )
{
    GC_UNUSED( replace );
    ::SetEnvironmentVariable( name, value );
    return 0;
}
#endif

int main(int /*argc*/, char* /*argv[]*/)
{
    // Setup the environment variables pointing out GenICam's home directory 
    // The following code assumes that an environment variable GENICAM_ROOT exists
    // Note that this variable is normally not present at runtime 
    std::string  MyGenICamRoot( getenv("GENICAM_ROOT") );

    // GENICAM_ROOT_VERSION holds the path to the GenICam root directory 
    // GENICAM_ROOT_VERSION expands to something like "GENICAM_ROOT_V1_0" depending on the GenICam version used
    // This variable must be present at runtime 
    setenv(GENICAM_ROOT_VERSION, MyGenICamRoot.c_str(), false );
    
    // GENICAM_CACHE holds a path to a location were cached XML files are stored
    // This location must be writable therefore it is normally different from the home directory 
    // were users without admin rights cannot write files.
    std::string  MyGenICamBCache( MyGenICamRoot );
    MyGenICamBCache += "/xml/GenApi/cache";
    setenv("GENICAM_CACHE", MyGenICamBCache.c_str(), false );
    
#if WIN32
    // The GenICam DLLs from the installation must be in the PATH. 
    // Note that SetDllDirectory is avaliable only under WinXP SP1 and higher
    std::string  MyGenICamBin( MyGenICamRoot );
    MyGenICamBin += "/bin/Win32_i86";
    ::SetDllDirectory( MyGenICamBin.c_str() );
#endif

    // Get a camera description file
    // This is an example for a very primitive camera description file
    const GenICam::gcstring CameraDescriptionFile =
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>                                         \n"
        "<RegisterDescription                                                               \n"
        "      ModelName=\"UsingGenApiExample\"                                             \n"
        "      VendorName=\"Generic\"                                                       \n"
        "      ToolTip=\"Sample nodes\"                                                     \n"
        "      StandardNameSpace=\"GEV\"                                                    \n"
        "      SchemaMajorVersion=\"1\"                                                     \n"
        "      SchemaMinorVersion=\"0\"                                                     \n"
        "      SchemaSubMinorVersion=\"1\"                                                  \n"
        "      MajorVersion=\"2\"                                                           \n"
        "      MinorVersion=\"3\"                                                           \n"
        "      SubMinorVersion=\"4\"                                                        \n"
        "      ProductGuid=\"55D10620-B3F2-428f-AA26-E613FDACB0F2\"                         \n"
        "      VersionGuid=\"5B673AA5-1C47-417e-959D-CB5445368A5A\"                         \n"
        "      xmlns=\"http://www.genicam.org/GenApi/Version_1_0\"                          \n"
        "      xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"                      \n"
        "      xsi:schemaLocation=\"http://www.genicam.org/GenApi/Version_1_0               \n"
        "            http://www.genicam.org/GenApi/GenApiSchema_Version_1_0.xsd\">          \n"
        "                                                                                   \n"
        "   <Integer Name=\"TheValue\">                                                      \n"
        "        <Value>0</Value>                                                            \n"
        "   </Integer>                                                                       \n"
        "                                                                                   \n"
        "</RegisterDescription>                                                             \n";


    // Create a camera access object and load the camera description file
    CNodeMapRef Camera;
    Camera._LoadXMLFromString(CameraDescriptionFile);

    // Get the Value node and access it
    CIntegerPtr ptrValue = Camera._GetNode("TheValue");

    // access the node
    assert( 0 == *ptrValue());
    *ptrValue = 42;
    int64_t Value = *ptrValue();
    assert( 42 == Value);

    printf("UsingGenApi_1 : GenICam worked all right.\n");

    return ! (42 == Value);

}




