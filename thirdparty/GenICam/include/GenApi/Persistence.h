//-----------------------------------------------------------------------------
//  (c) 2007 by National Instruments
//  Project: GenApi
//  Author:  Eric Gross
//  $Header: /cvs/genicam/genicam/library/CPP/include/GenApi/Persistence.h,v 1.3.2.2 2009/02/05 11:18:28 hartmut_nebelung Exp $
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
\brief    Definition of interface IPersistScript and class CFeatureBag
\ingroup GenApi_PublicUtilities
*/

#ifndef _GENICAM_PERSISTENCE_H
#define _GENICAM_PERSISTENCE_H

#include <GenApi/Types.h>
#include <GenApi/Pointer.h>
#include <GenApi/GenApiVersion.h>
#include "../GenICam.h"
#include <list>
#include <iostream>

using namespace GenICam;

namespace GenApi
{

    //! Basic interface to persist values to
    interface IPersistScript 
    {        
        //! sets information about the node map
        virtual void SetInfo(gcstring &Info) = 0;

        //! Stores a feature 
        virtual void PersistFeature(IValue& item) = 0;
    };

    //! Bag holding streamable features of a nodetree
    class GENAPI_DECL CFeatureBag : public IPersistScript 
    {
    public:
        //! sets information about the node map
        virtual void SetInfo(gcstring &Info);

        //! Stores a feature 
        virtual void PersistFeature(IValue& item);
        
        //! Loads the features from the bag to the node tree
        void LoadFromBag(INodeMap *pNodeMap);

        //! Stores the streamable nodes to this fature bag
        void StoreToBag(INodeMap *pNodeMap);

        //! fills the bag from a stream
        friend std::istream& operator >>(std::istream &is, CFeatureBag &FeatureBag);

        //! puts the bag into a stream
        friend std::ostream& operator <<(std::ostream &os, const CFeatureBag &FeatureBag);

    private:
        //! The features are stored in a list of string pairs = {Name, Value}
        gcstring_vector m_Names;
        
        //! The features are stored in a list of string pairs = {Name, Value}
        gcstring_vector m_Values;

        //! String describing the node map
        gcstring m_Info;
    };

    //! Helper fundtion ignoring lines starting with comment character '#'
    inline std::istream& EatComments(std::istream &is)
    {
        if( is.eof() )
            return is;

        char FirstCharacter;
        FirstCharacter = (char)is.peek();
        
        while( FirstCharacter == '#' )
        {
            is.ignore(1024, '\n');
            FirstCharacter = (char)is.peek();
        }
        return is;
    }

    
    //! the magic GUID which indicates that the file is a GenApi stream file Must be the first entry
    #define GENAPI_PERSISTENCE_MAGIC   "{05D8C294-F295-4dfb-9D01-096BD04049F4}"

    //! reads in presistent data from a stream
    inline std::istream& operator >>(std::istream &is, CFeatureBag &FeatureBag)
    {
        if( is.eof() )
            throw RUNTIME_EXCEPTION("The stream is eof");

        FeatureBag.m_Names.clear();
        FeatureBag.m_Values.clear();

        const int BufferSize = 1024;
        char Buffer[BufferSize] = {0};
        char Name[BufferSize] = {0};
        gcstring Value("");
        
        // Check the magic
        is.getline(Buffer, BufferSize, '\n');
        gcstring FirstLine(Buffer);
        gcstring MagicGUID(GENAPI_PERSISTENCE_MAGIC);
        if( gcstring::npos == FirstLine.find(MagicGUID) )
            throw RUNTIME_EXCEPTION("The stream is not a GenApi feature stream since it is missing the magic GUID in the first line");

        EatComments( is );
        while( !is.eof() )
        {
            is.getline(Name, BufferSize, '\t');
            if (is.fail())  // if reading from stream failed -> stop reading!
                break;

            GenICam::getline(is, Value);
            if (is.fail())  // if reading from stream failed -> stop reading!
                break;

            FeatureBag.m_Names.push_back(Name);
            FeatureBag.m_Values.push_back(Value); 

            Name[0] = '\0';
            Value = "";
            EatComments( is );
        }
        return is;
    }

    //! writes out presistent data to a stream 
    inline std::ostream& operator <<(std::ostream &os, const CFeatureBag &FeatureBag)
    {
#pragma warning( push )
#pragma warning ( disable : 4068 ) // unknown pragma; refers to BullsEyeCoverage
        os << "# " GENAPI_PERSISTENCE_MAGIC "\n";
        os << "# GenApi persistence file (version " << GENAPI_VERSION_MAJOR << "." << GENAPI_VERSION_MINOR << "." << GENAPI_VERSION_SUBMINOR << ")\n";
        os << "# " << FeatureBag.m_Info << "\n";
        
        gcstring_vector::const_iterator pName = FeatureBag.m_Names.begin();
        gcstring_vector::const_iterator pValue = FeatureBag.m_Values.begin();
        for( /**/ ; pName != FeatureBag.m_Names.end()
                      #pragma BullseyeCoverage off
                      && pValue != FeatureBag.m_Values.end();
                      #pragma BullseyeCoverage on
                      pName++, pValue++ ) 
        {
            gcstring Name( *pName );
            gcstring Value( *pValue );
            os << Name << "\t" << Value << "\n";    
        }       
        return os;
#pragma warning (pop )
    }

} 

#endif //_GENICAM_PERSISTENCE_H
