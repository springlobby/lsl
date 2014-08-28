#ifndef LSL_MMOPTIONSORAPPER_H_
#define LSL_MMOPTIONSORAPPER_H_

#include <vector>
#include <utility>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>

#include <lslutils/type_forwards.h>
#include <lslunitsync/unitsync.h> // struct GameOptions
#include "enum.h"

namespace LSL {

struct mmOptionSection;

struct dummyConfig {};

//! \todo needs deep copy
class mmSectionTree
{
    public:
        mmSectionTree();
        ~mmSectionTree();

        void AddSection( const mmOptionSection& section );
        mmOptionSection GetSection( const std::string& key );

        typedef std::vector< mmOptionSection > SectionVector;
//        SectionVector GetSectionVector();
        void Clear();

    protected:
        //map key -> option
        typedef std::map< std::string, mmOptionSection > SectionMap;
        SectionMap m_section_map;
		typedef dummyConfig ConfigType;
		boost::shared_ptr<ConfigType> m_tree;

        void AddSection ( const std::string& path, const mmOptionSection& section );
        std::string FindParentpath ( const std::string& parent_key );
        bool FindRecursive( const std::string& parent_key, std::string& path );
};

class OptionsWrapper
{
public:
    //! public types
    typedef std::pair < std::string,std::string> stringPair;
    typedef std::pair < std::string, stringPair> stringTriple;
    typedef std::vector<stringPair> stringPairVec;
    typedef std::vector<stringTriple> stringTripleVec;
    typedef std::map<std::string,std::string> stringMap;
    typedef std::map<int, GameOptions> GameOptionsMap;
    typedef std::map<int, mmSectionTree> mmSectionTreeMap;

    //! enum to differentiate option category easily at runtime
    enum GameOption{
        PrivateOptions      = 3,
        EngineOption        = 2,
        MapOption           = 1,
        ModOption           = 0,
        LastOption          = 6
    };// should reflect: optionCategoriesCount

	//does nothing
	OptionsWrapper();
	virtual ~OptionsWrapper();
	//! just calls loadOptions(MapOption,mapname)
	bool loadMapOptions(const std::string& mapname);

	bool loadAIOptions( const std::string& modname, int aiindex, const std::string& ainick );

	int GetAIOptionIndex( const std::string& nick ) const;

	//! load corresponding options through unitsync calls
	/*!
	 * the containers for corresponding flag are recreated and then gets the number of options from unitsync
	 * and adds them one by one  to the appropiate container
	 * \param flag decides which type of option to load
	 * \param name Mod/Mapname
	 * \return true if load successful, false otherwise
	 */
    bool loadOptions(GameOption flag, const std::string& name);
	//! checks if given key can be found in specified container
	/*!
	 * \param key the key that should be checked for existance in containers
	 * \param flag which GameOption conatiner should be searched
	 * \param showError if true displays a messagebox if duplicate key is found
	 * \param optType will contain the corresponding OptionType if key is found, opt_undefined otherwise
	 * \return true if key is found, false otherwise
	 */
    bool keyExists(const std::string& key, const GameOption flag, const bool showError, Enum::OptionType& optType) const;
	//! checks if given key can be found in all containers
	/*!
	 * \param key the key that should be checked for existance in containers
	 * \return true if key is found, false otherwise
	 */
	bool keyExists(const std::string& key ) const;
	//! checks which container this key belongs to
	/*!
	 * \param key the key that should be checked for existance in containers
	 * \return they container section
	 */
	GameOption GetSection( const std::string& key ) const;
	//! given a vector of key/value pairs sets the appropiate options to new values
	/*!	Every new value is tested for meeting boundary conditions, type, etc.
	 * If test fails error is logged and false is returned.
	 * \param values the std::stringPairVec containing key / new value pairs
	 * \param flag which OptionType is to be processed
	 * \return false if ANY error occured, true otherwise
	 */
    bool setOptions(stringPairVec* values,GameOption flag);
	//! get all options of one GameOption
	/*! the output has the following format: < std::string , Pair < std::string , std::string > >
	 * meaning < key , < name , value > >
	 * \param triples this will contain the options after the function
	 * \param flag which OptionType is to be processed
	 */
    stringTripleVec getOptions( GameOption flag ) const;
	//! similar to getOptions, instead of vector a map is used and the name is not stored
	std::map<std::string,std::string> getOptionsMap(GameOption) const;
	//! recreates ALL containers
	void unLoadOptions();
	//! recreates the containers of corresponding flag
	void unLoadOptions(GameOption flag);

	//! returns value of specified key
	/*! searches all containers for key
	 * \return value of key if key found, "" otherwise
	 */
	std::string getSingleValue(const std::string& key) const;
	//! returns value of specified key
	/*! searches containers of type flag for key
	 * \return value of key if key found, "" otherwise
	 */

	std::string getSingleValue(const std::string& key, GameOption flag) const;

	std::string getDefaultValue(const std::string& key, GameOption flag) const;

	//! sets a single option in specified container
	/*! \return true if success, false otherwise */
	bool setSingleOption(const std::string& key, const std::string& value, GameOption modmapFlag);
	//! same as before, but tries all containers
	bool setSingleOption(const std::string& key, const std::string& value);

	//! returns the option type of specified key (all containers are tried)
    Enum::OptionType GetSingleOptionType (const std::string& key) const;

	//!returns the cbx_choice associated w current listoption
	std::string GetNameListOptValue(const std::string& key, GameOption flag) const;

	//! returns the listitem key associated with listitem name
	std::string GetNameListOptItemKey(const std::string& optkey, const std::string& itemname, GameOption flag) const;

	GameOptionsMap m_opts;

	//! after loading sections into map, parse them into tree
    void ParseSectionMap( mmSectionTree& section_tree, const OptionMapSection& section_map );

	//! Merge this another wrapper's options into this one, with the other'soptions taking precendence
	bool MergeOptions( const OptionsWrapper& other, GameOption merge_into );
protected:
	//! used for code clarity in setOptions()
    bool setSingleOptionTypeSwitch(const std::string& key, const std::string& value, GameOption modmapFlag, Enum::OptionType optType);

	mmSectionTreeMap m_sections;

	//! a map that connects the ai nick with it's set of options
	std::map<std::string, int> m_ais_indexes;

	typedef GameOptionsMap::const_iterator
        GameOptionsMapCIter;

};

} // namespace LSL {

/**
 * \file optionswrapper.h
 * \section LICENSE
Copyright 2012 by The libSpringLobby team. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#endif /*LSL_MMOPTIONSORAPPER_H_*/
