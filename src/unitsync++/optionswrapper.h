#ifndef LSL_MMOPTIONSORAPPER_H_
#define LSL_MMOPTIONSORAPPER_H_

#include <vector>
#include <utility>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>

#include <utils/type_forwards.h>
#include "enum.h"

class wxFileConfig;

namespace LSL {

class mmOptionSection;

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
        typedef wxFileConfig ConfigType;
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
        ModCustomizations   = 4,
        SkirmishOptions     = 5,
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
	 * \param extra_filename used for loading arbitrary lua tables from inside the given mod/map
	 * \return true if load successful, false otherwise
	 */
    bool loadOptions(GameOption flag, const std::string& name, const std::string& extra_filename = "" );
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

#endif /*LSL_MMOPTIONSORAPPER_H_*/

/**
    This file is part of SpringLobby,
    Copyright (C) 2007-2011

    SpringLobby is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as published by
    the Free Software Foundation.

    SpringLobby is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SpringLobby.  If not, see <http://www.gnu.org/licenses/>.
**/

