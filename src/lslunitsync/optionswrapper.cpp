#include "optionswrapper.h"

#include "unitsync.h"
#include <lslutils/conversion.h>
#include <lslutils/debug.h>
#include <lslutils/logging.h>

#include <stdexcept>
#include <clocale>

namespace LSL {

OptionsWrapper::OptionsWrapper()
{
	unLoadOptions();
    loadOptions( Enum::EngineOption, "" );
    loadOptions( Enum::PrivateOptions,"" );
}

void OptionsWrapper::unLoadOptions()
{
	for (int i = 0; i < Enum::LastOption; ++i)
	{
		unLoadOptions( (Enum::GameOption)i );
	}
}

void OptionsWrapper::unLoadOptions(Enum::GameOption i)
{
  GameOptions empty;
  m_opts[i] = empty;

	mmSectionTreeMap::iterator itor = m_sections.find( i );
	if ( itor != m_sections.end() ) m_sections.erase( itor );
}

OptionsWrapper::~OptionsWrapper()
{
}

bool OptionsWrapper::loadMapOptions( const std::string& mapname)
{
	return loadOptions(Enum::MapOption,mapname);
}

Enum::OptionType OptionsWrapper::GetSingleOptionType ( const std::string& key) const
{
    Enum::OptionType type = Enum::opt_undefined;
	for ( int g = 0; g < Enum::LastOption; g++ )
	{
		if (keyExists(key,(Enum::GameOption)g,false,type))
			return type;
	}
    return Enum::opt_undefined;
}

bool OptionsWrapper::loadAIOptions( const std::string& modname, int aiindex,const std::string& ainame )
{
	int mapindex = m_ais_indexes[ainame];
	if ( mapindex == 0 ) mapindex = m_ais_indexes.size() + Enum::LastOption;
	m_ais_indexes[ainame] = mapindex;
	unLoadOptions((Enum::GameOption)mapindex);
	try
	{
		GameOptions opt = usync().GetAIOptions( modname, aiindex );
		ParseSectionMap( m_sections[mapindex], opt.section_map );
		m_opts[mapindex] = opt;
	} catch (...)
	{
		return false;
	}
	return true;
}

int OptionsWrapper::GetAIOptionIndex( const std::string& nick ) const
{
	std::map<std::string,int>::const_iterator itor = m_ais_indexes.find(nick);
	int pos = -1;
	if ( itor != m_ais_indexes.end() ) pos = itor->second;
	return pos;
}

bool OptionsWrapper::loadOptions( Enum::GameOption modmapFlag, const std::string& name)
{
	unLoadOptions(modmapFlag);
	GameOptions opt;
	switch (modmapFlag)
	{
	    default:
            break;
		case Enum::MapOption:
			try
			{
                opt = usync().GetMapOptions(name);
                ParseSectionMap( m_sections[modmapFlag], opt.section_map );
			}
			catch(...)
			{
                LslError("Could not load map options");
				return false;
			}
			break;

		case Enum::ModOption:
			try
			{
                opt = usync().GetModOptions(name);
                ParseSectionMap( m_sections[modmapFlag], opt.section_map );
			}
			catch(...)
			{
                LslError("Could not load game options");
				return false;
			}
			break;

        case Enum::EngineOption: {
            //TODO Fixed,random and so forth are intls
            mmOptionList startpos( "Start Position Type", "startpostype", "How players will select where to be spawned in the map\n0: fixed map positions\n1: random map positions\n2: choose in game\n3: choose in the lobby before starting", "0" );
            startpos.addItem( "0", "Fixed", "Use the start positions defined in the map, the positions will be assigned incrementally from the team with lowest number to highest");
            startpos.addItem( "1", "Random", "Use the start positions defined in the map, the positions will be assigned randomly");
            startpos.addItem( "2", "Choose in-game", "Players will be able to pick their own starting point right before the game starts, optionally limited by a bounding box defined by the host");
            startpos.addItem( "3", "Choose before game", "The host will place each player's start position in the map preview before the game is launched");
            opt.list_map["startpostype"] = startpos;
            break;
        }

        case Enum::PrivateOptions: {
            opt.string_map["restrictions"] = mmOptionString("List of restricted units", "restrictedunits", "Units in this list won't be available in game", "", 0 ); // tab separated list
            opt.string_map["mapname"] = mmOptionString("Map name", "mapname", "Map name", "", 0 );
            break;
        }
	}
	m_opts[modmapFlag] = opt;
	return true;
}

Enum::GameOption OptionsWrapper::GetSection( const std::string& key ) const
{
	Enum::GameOption ret = Enum::LastOption;
	bool found = false;
	for ( int flag = 0; flag < Enum::PrivateOptions; flag++ )
	{
        Enum::OptionType optType = Enum::opt_undefined;
		found = keyExists( key, (Enum::GameOption)flag, false, optType );
		if ( found )
		{
			 ret = (Enum::GameOption)flag;
			 break;
		}
	}
	return ret;
}

bool OptionsWrapper::keyExists( const std::string& key ) const
{
	bool found = false;
	for ( int flag = 0; flag < Enum::PrivateOptions; flag++ )
	{
        Enum::OptionType optType = Enum::opt_undefined;
		found = keyExists( key, (Enum::GameOption)flag, false, optType );
		if ( found ) break;
	}
	return found;
}

bool OptionsWrapper::keyExists( const std::string& key, const Enum::GameOption modmapFlag, bool showError, Enum::OptionType& optType ) const
{
    //std::string duplicateKeyError = "Please contact the game's author and tell him\nto use unique keys in his ModOptions.lua";
	bool exists = false;
    optType = Enum::opt_undefined;
    GameOptionsMap::const_iterator optIt = m_opts.find((int)modmapFlag);
    if ( optIt == m_opts.end() )
        return false;
	const GameOptions& gameoptions = optIt->second;
	if (  gameoptions.list_map.find(key) !=  gameoptions.list_map.end())
	{
        optType = Enum::opt_list;
		exists = true;
	}
	else if ( gameoptions.string_map.find(key) !=  gameoptions.string_map.end())
	{
        optType = Enum::opt_string;
		exists = true;
	}
	else if ( gameoptions.bool_map.find(key) !=  gameoptions.bool_map.end())
	{
        optType = Enum::opt_bool;
		exists = true;
	}
	else if ( gameoptions.float_map.find(key)!=  gameoptions.float_map.end())
	{
        optType = Enum::opt_float;
		exists = true;
	}
	else if ( gameoptions.section_map.find(key)!=  gameoptions.section_map.end())
	{
        optType = Enum::opt_section;
		exists = true;
	}
	if (exists && showError)
	{
        //TODO STH
//		customMessageBoxNoModal(SL_MAIN_ICON,duplicateKeyError, "Mod/map option error",wxOK);
        LslWarning("duplicate key in mapmodoptions");
		return false;
	}
	else if ( exists && !showError )
	{
        return true;
	}
	else
		return false;
}

bool OptionsWrapper::setOptions(stringPairVec* options, Enum::GameOption modmapFlag)
{
    for (stringPairVec::const_iterator it = options->begin(); it != options->end(); ++it)
	{
		std::string key = it->first;
		std::string value = it->second;

		//we don't want to add a key that doesn't already exists
        Enum::OptionType optType = Enum::opt_undefined;
		if(!keyExists(key,modmapFlag,false,optType))
			return false;
		else
		{
			if ( !setSingleOptionTypeSwitch( key, value, modmapFlag, optType) )
				return false;
		}
	}
	return true;
}

OptionsWrapper::stringTripleVec OptionsWrapper::getOptions( Enum::GameOption modmapFlag) const
{
    stringTripleVec list;
    GameOptionsMapCIter optIt = m_opts.find((int)modmapFlag);
    if ( optIt != m_opts.end() ) {
        const GameOptions& gameoptions = optIt->second;
        for (OptionMapBoolConstIter it = gameoptions.bool_map.begin(); it != gameoptions.bool_map.end(); ++it) {
            list.push_back( stringTriple( (*it).first, stringPair ( it->second.name , Util::ToString(it->second.value) ) ) );
        }

        for (OptionMapStringConstIter it = gameoptions.string_map.begin(); it != gameoptions.string_map.end(); ++it) {
            list.push_back( stringTriple( (*it).first, stringPair ( it->second.name, it->second.value) ) );
        }

        for (OptionMapFloatConstIter it = gameoptions.float_map.begin(); it != gameoptions.float_map.end(); ++it) {
            list.push_back( stringTriple( (*it).first, stringPair ( it->second.name, Util::ToString(it->second.value) ) ) );
        }

        for (OptionMapListConstIter it = gameoptions.list_map.begin(); it != gameoptions.list_map.end(); ++it) {
            list.push_back( stringTriple( (*it).first, stringPair ( it->second.name, it->second.value ) ) );
        }
    }
	return list;
}

std::map<std::string,std::string> LSL::OptionsWrapper::getOptionsMap( Enum::GameOption modmapFlag ) const
{
    std::map<std::string,std::string> map;
    GameOptionsMapCIter optIt = m_opts.find((int)modmapFlag);
    if ( optIt != m_opts.end() ) {
        const GameOptions& gameoptions = optIt->second;
        for (OptionMapBoolConstIter it = gameoptions.bool_map.begin(); it != gameoptions.bool_map.end(); ++it) {
            map[it->first] =  Util::ToString(it->second.value);
        }

        for (OptionMapStringConstIter it = gameoptions.string_map.begin(); it != gameoptions.string_map.end(); ++it) {
            map[it->first] = it->second.value;
        }

        for (OptionMapFloatConstIter it = gameoptions.float_map.begin(); it != gameoptions.float_map.end(); ++it) {
            map[it->first] = Util::ToString(it->second.value);
        }

        for (OptionMapListConstIter it = gameoptions.list_map.begin(); it != gameoptions.list_map.end(); ++it) {
            map[it->first] = it->second.value;
        }
    }
	return map;
}

bool OptionsWrapper::setSingleOption( const std::string& key, const std::string& value,Enum::GameOption modmapFlag)
{
	Enum::OptionType optType = Enum::opt_undefined;
	if (keyExists( key, modmapFlag, false, optType )) {
		return setSingleOptionTypeSwitch(key,value,modmapFlag,optType);
	}
	return false;
}

bool OptionsWrapper::setSingleOption( const std::string& key, const std::string& value )
{
	Enum::OptionType optType = Enum::opt_undefined;
	if (keyExists(key,Enum::ModOption,false,optType)) {
		return setSingleOptionTypeSwitch(key,value,Enum::ModOption,optType);
	}
	return false;
}

std::string OptionsWrapper::getSingleValue( const std::string& key) const
{
	for ( int g = 0; g < Enum::LastOption; g++ )
	{
		const std::string tmp = getSingleValue(key, (Enum::GameOption)g);
        if (tmp != "")
			return tmp;
	}
    return "";
}
template < class MapType >
static inline typename MapType::mapped_type GetItem( const MapType& map, const typename MapType::key_type& key )
{
    typename MapType::const_iterator mapIt = map.find(key);
    if ( mapIt != map.end() )
        return mapIt->second;
    else
        return typename MapType::mapped_type();
}

std::string OptionsWrapper::getSingleValue( const std::string& key, Enum::GameOption modmapFlag) const
{
    Enum::OptionType optType = Enum::opt_undefined;

	if ( keyExists(key,modmapFlag,false,optType) )
	{
        GameOptionsMapCIter optIt = m_opts.find((int)modmapFlag);
        if ( optIt == m_opts.end() )
            return "";

        const GameOptions& tempOpt = optIt->second;
		switch (optType)
		{
        case Enum::opt_float:
            return Util::ToString( GetItem( tempOpt.float_map, key ).value );
        case Enum::opt_bool:
            return Util::ToString( GetItem( tempOpt.bool_map, key ).value );
        case Enum::opt_string:
			return  GetItem( tempOpt.string_map, key ).value ;
        case Enum::opt_list:
			return GetItem( tempOpt.list_map, key ).value;
        case Enum::opt_undefined:
        default:
            return "";
		}
	}
    return "";
}

std::string OptionsWrapper::getDefaultValue( const std::string& key, Enum::GameOption modmapFlag) const
{
    Enum::OptionType optType = Enum::opt_undefined;
	std::string ret;
	if ( keyExists(key,modmapFlag,false,optType) )
	{
			//purposefully create a copy, no better idea
        GameOptionsMapCIter optIt = m_opts.find((int)modmapFlag);
        if ( optIt == m_opts.end() )
            return "";

        const GameOptions& tempOpt = optIt->second;
		switch ( optType )
		{
			{
            case Enum::opt_bool:
                ret = Util::ToString( GetItem( tempOpt.bool_map, key ).def );
				break;
			}
            case Enum::opt_float:
			{
                ret = Util::ToString( GetItem( tempOpt.float_map, key ).def );
				break;
			}
            case Enum::opt_string:
			{
				ret = GetItem( tempOpt.string_map, key ).def;
				break;
			}
            case Enum::opt_list:
			{
				ret = GetItem( tempOpt.list_map, key ).def;
				break;
			}
			default:
			{
				break;
			}
		}
	}
	return ret;
}

bool  LSL::OptionsWrapper::setSingleOptionTypeSwitch( const std::string& key, const std::string& value, Enum::GameOption modmapFlag, Enum::OptionType optType)
{
	GameOptions& gameoptions = m_opts[modmapFlag];
	switch (optType)
	{
        case Enum::opt_float :
		{
			//temp set to C locale cause we get '.' as decimal seperator over the net
			const char* old_locale = std::setlocale(LC_NUMERIC, "C");
			//test if min < val < max
			const double d_val = Util::FromString<double>( value );
			std::setlocale(LC_NUMERIC, old_locale);
			if( d_val < (gameoptions.float_map)[key].min || d_val > (gameoptions.float_map)[key].max ) {
				LslWarning("received number %f option %s exceeds boundaries %f %f", d_val, key.c_str(), (gameoptions.float_map)[key].min, (gameoptions.float_map)[key].max);
				return false;
			}
			else
				(gameoptions.float_map)[key].value = d_val;
			break;
		}
        case Enum::opt_bool :
		{
			const long l_val = Util::FromString<long>( value );
			if( l_val != 1 && l_val != 0 )
			{
                LslWarning("recieved bool option that is neither 0 or 1");
				return false;
			}
			else
				(gameoptions.bool_map)[key].value = bool(l_val);
			break;
		}
        case Enum::opt_string :
		{
			// test if maxlength isn't exceeded
			unsigned int max_length = (gameoptions.string_map)[key].max_len;
            if ( ( max_length != 0 ) && ( value.length() > max_length )  )
			{
                LslWarning("recieved string option exceeds max_len");
				return false;
			}
			else
				(gameoptions.string_map)[key].value = value;
			break;
		}
        case Enum::opt_list :
		{
			// test if valid value, aka is in list
			int listitemcount = (gameoptions.list_map)[key].listitems.size();
			bool valid_string = false;
			int j = 0;
			for (; j < listitemcount; ++j)
			{
				if ( (gameoptions.list_map)[key].listitems[j].key == value)
				{
					valid_string = true;
					break;
				}
			}

			if (valid_string)
			{
			    //LOOKATME (koshi) if there's a problem with list modoption look here first
				(gameoptions.list_map)[key].value = (gameoptions.list_map)[key].listitems[j].key;
				(gameoptions.list_map)[key].cur_choice_index = j;
			}
			else
			{
                LslWarning("received list option \"%s\" is not valid", key.c_str());
				return false;
			}
			break;
		}
		default:
			return false;
	}
	//if we made it here, all is good
	return true;
}

std::string OptionsWrapper::GetNameListOptValue( const std::string& key, Enum::GameOption flag) const
{
    Enum::OptionType optType;
	if ( keyExists(key,flag,false, optType) )
	{
        if ( optType == Enum::opt_list)
		{
            GameOptionsMapCIter optIt = m_opts.find((int)flag);
            if ( optIt == m_opts.end() )
                return "";

            GameOptions tempOpt = optIt->second;
			return ( (tempOpt.list_map)[key].cbx_choices[ (tempOpt.list_map)[key].cur_choice_index ] );
		}
	}
	// at this point retrieval failed
    return "";
}

std::string OptionsWrapper::GetNameListOptItemKey( const std::string& optkey, const std::string& itemname, Enum::GameOption flag) const
{
    Enum::OptionType optType;
	if ( keyExists(optkey,flag,false, optType) )
	{
        if ( optType == Enum::opt_list)
		{
            GameOptionsMapCIter optIt = m_opts.find((int)flag);
            if ( optIt == m_opts.end() )
                return "";

            GameOptions tempOpt = optIt->second;
			for (ListItemVec::const_iterator it = (tempOpt.list_map)[optkey].listitems.begin(); it != (tempOpt.list_map)[optkey].listitems.end(); ++it)
			{
				if (it->name == itemname)
					return it->key;
			}
		}
	}

	// at this point retrieval failed
    return "";
}

bool OptionsWrapper::MergeOptions( const OptionsWrapper& other, Enum::GameOption merge_into )
{
    GameOptionsMapCIter other_it = other.m_opts.begin();
    for ( ; other_it != other.m_opts.end(); ++other_it ) {
        const GameOptions& other_opts = other_it->second;
        //const GameOption other_id = (const GameOption)other_it->first; //TODO (koshi) what was this supposed to be used for?

        for (OptionMapBoolConstIter it = other_opts.bool_map.begin(); it != other_opts.bool_map.end();++it ) {
            m_opts[merge_into].bool_map[it->first] = it->second;
        }

        for ( OptionMapFloatConstIter it = other_opts.float_map.begin(); it != other_opts.float_map.end(); ++it ) {
            m_opts[merge_into].float_map[it->first] = it->second;
        }

        for ( OptionMapListConstIter it = other_opts.list_map.begin(); it != other_opts.list_map.end(); ++it ){
            m_opts[merge_into].list_map[it->first] = it->second;
        }

        for ( OptionMapStringConstIter it = other_opts.string_map.begin(); it != other_opts.string_map.end(); ++it ) {
            m_opts[merge_into].string_map[it->first] = it->second;
        }
    }
    return true;
}

void OptionsWrapper::ParseSectionMap( mmSectionTree& section_tree, const OptionMapSection& section_map )
{

    // map child-key <-> parent-key
    typedef std::map<std::string,std::string> RelationMap;
	typedef std::map<std::string,std::string>::iterator RelationMapIter;
    RelationMap relation_map;

    //setup relation map
    for ( OptionMapSectionConstIter it = section_map.begin(); it != section_map.end(); ++it )
    {
        relation_map[it->second.key] = it->second.section;
    }

    RelationMapIter rit = relation_map.begin();
    // no more items in the map means we've added them all
    while ( !relation_map.empty() )
    {
        RelationMapIter rit_next = rit; // in case we need to delete
        ++rit_next;

        if ( relation_map.find(rit->second) == relation_map.end() )
        {
            //either we already added this sections parent or it's a root section
            OptionMapSectionConstIter section = section_map.find(rit->first);
            assert ( section != section_map.end() );
                section_tree.AddSection( section->second );


              //we're done with this section, so remove it
            relation_map.erase(rit);
        }

        rit = rit_next;

        //we've reached the end of the map, restart at beginning
        if ( rit == relation_map.end() )
            rit = relation_map.begin();
    }

}

const std::string tree_sep = "/";

mmSectionTree::mmSectionTree()
	: m_tree ( new ConfigType() )
{
	//this class is basically nonfunctional atm
	//FIXME: assert( false );
}

mmSectionTree::~mmSectionTree()
{
    #ifndef NDEBUG
//		m_tree->Flush();
	#else //no need to clutter tempfile directory if we're not debugging
//		m_tree->DeleteAll();
    #endif
}

void mmSectionTree::AddSection ( const std::string& parentpath, const mmOptionSection& section )
{
//FIXME
//	std::string fullpath = parentpath + tree_sep + section.key + tree_sep;
//	m_tree->Write( fullpath + "key", section.key );
	#ifndef NDEBUG
//		m_tree->Flush();
	#endif
}
void mmSectionTree::AddSection( const mmOptionSection& section)
{
	//m_section_map[section.key] = section;
	std::string name = section.section;
	if ( section.section == Constants::nosection_name )
	{
		AddSection( tree_sep, section );
	}
	else
	{
		std::string parent = FindParentpath( section.section );
		AddSection( parent, section );
	}
}

bool mmSectionTree::FindRecursive( const std::string& /*parent_key*/, std::string& /*path */)
{
//    std::string current;
//    long cur_index;

//    //search current level first before recursing
//    bool cont = m_tree->GetFirstGroup( current, cur_index );
//    while ( cont )
//    {
//        if ( current.EndsWith( parent_key ) ) {
//            path = current;
//            return true;
//        }
//        cont = m_tree->GetNextGroup( current, cur_index );
//    }

//    //we need to recurse into sub-paths
//    cont = m_tree->GetFirstGroup( current, cur_index );
//    while ( cont )
//    {
//        std::string old_path = m_tree->GetPath();
//        m_tree->SetPath( old_path + "/" + current );
//        if ( FindRecursive( parent_key,  path ) )
//            return true;
//        m_tree->SetPath( old_path );
//        cont = m_tree->GetNextGroup( current, cur_index );
//    }
    return false;
}

std::string mmSectionTree::FindParentpath ( const std::string& parent_key )
{
	std::string path = tree_sep;
    if ( FindRecursive( parent_key, path ) )
        return path;
    else
        return "";
}

void mmSectionTree::Clear()
{
    m_section_map.clear();
//    m_tree->DeleteAll();
}

} // namespace LSL {
