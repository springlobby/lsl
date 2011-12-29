#include "loader.h"

#include <boost/extension/shared_library.hpp>
#include <string>

#include "c_api.h"
#include "function_ptr.h"

namespace LSL {

void UnitsyncFunctionLoader::LuaParser(SpringUnitSyncLib *s)
{
    GetLibFuncPtr( s->m_libhandle, "lpClose",                   s->m_parser_close );
    GetLibFuncPtr( s->m_libhandle, "lpOpenFile",				s->m_parser_open_file );
    GetLibFuncPtr( s->m_libhandle, "lpOpenSource",				s->m_parser_open_source );
    GetLibFuncPtr( s->m_libhandle, "lpExecute",					s->m_parser_execute );
    GetLibFuncPtr( s->m_libhandle, "lpErrorLog",				s->m_parser_error_log );

    GetLibFuncPtr( s->m_libhandle, "lpAddTableInt",				s->m_parser_add_table_int );
    GetLibFuncPtr( s->m_libhandle, "lpAddTableStr",				s->m_parser_add_table_string );
    GetLibFuncPtr( s->m_libhandle, "lpEndTable",				s->m_parser_end_table );
    GetLibFuncPtr( s->m_libhandle, "lpAddIntKeyIntVal",			s->m_parser_add_int_key_int_value );
    GetLibFuncPtr( s->m_libhandle, "lpAddStrKeyIntVal",			s->m_parser_add_string_key_int_value );
    GetLibFuncPtr( s->m_libhandle, "lpAddIntKeyBoolVal",		s->m_parser_add_int_key_bool_value );
    GetLibFuncPtr( s->m_libhandle, "lpAddStrKeyBoolVal",		s->m_parser_add_string_key_bool_value );
    GetLibFuncPtr( s->m_libhandle, "lpAddIntKeyFloatVal",		s->m_parser_add_int_key_float_value );
    GetLibFuncPtr( s->m_libhandle, "lpAddStrKeyFloatVal",		s->m_parser_add_string_key_float_value );
    GetLibFuncPtr( s->m_libhandle, "lpAddIntKeyStrVal",			s->m_parser_add_int_key_string_value );
    GetLibFuncPtr( s->m_libhandle, "lpAddStrKeyStrVal",			s->m_parser_add_string_key_string_value );

    GetLibFuncPtr( s->m_libhandle, "lpRootTable",				s->m_parser_root_table );
    GetLibFuncPtr( s->m_libhandle, "lpRootTableExpr",			s->m_parser_root_table_expression );
    GetLibFuncPtr( s->m_libhandle, "lpSubTableInt",				s->m_parser_sub_table_int );
    GetLibFuncPtr( s->m_libhandle, "lpSubTableStr",				s->m_parser_sub_table_string );
    GetLibFuncPtr( s->m_libhandle, "lpSubTableExpr",			s->m_parser_sub_table_expression );
    GetLibFuncPtr( s->m_libhandle, "lpPopTable",				s->m_parser_pop_table );

    GetLibFuncPtr( s->m_libhandle, "lpGetKeyExistsInt",			s->m_parser_key_int_exists );
    GetLibFuncPtr( s->m_libhandle, "lpGetKeyExistsStr",			s->m_parser_key_string_exists );

    GetLibFuncPtr( s->m_libhandle, "lpGetIntKeyType",			s->m_parser_int_key_get_type );
    GetLibFuncPtr( s->m_libhandle, "lpGetStrKeyType",			s->m_parser_string_key_get_type );

    GetLibFuncPtr( s->m_libhandle, "lpGetIntKeyListCount",		s->m_parser_int_key_get_list_count );
    GetLibFuncPtr( s->m_libhandle, "lpGetIntKeyListEntry",		s->m_parser_int_key_get_list_entry );
    GetLibFuncPtr( s->m_libhandle, "lpGetStrKeyListCount",		s->m_parser_string_key_get_list_count );
    GetLibFuncPtr( s->m_libhandle, "lpGetStrKeyListEntry",		s->m_parser_string_key_get_list_entry );

    GetLibFuncPtr( s->m_libhandle, "lpGetIntKeyIntVal",			s->m_parser_int_key_get_int_value );
    GetLibFuncPtr( s->m_libhandle, "lpGetStrKeyIntVal",			s->m_parser_string_key_get_int_value );
    GetLibFuncPtr( s->m_libhandle, "lpGetIntKeyBoolVal",		s->m_parser_int_key_get_bool_value );
    GetLibFuncPtr( s->m_libhandle, "lpGetStrKeyBoolVal",		s->m_parser_string_key_get_bool_value );
    GetLibFuncPtr( s->m_libhandle, "lpGetIntKeyFloatVal",       s->m_parser_int_key_get_float_value );
    GetLibFuncPtr( s->m_libhandle, "lpGetStrKeyFloatVal",		s->m_parser_string_key_get_float_value );
    GetLibFuncPtr( s->m_libhandle, "lpGetIntKeyStrVal",			s->m_parser_int_key_get_string_value );
    GetLibFuncPtr( s->m_libhandle, "lpGetStrKeyStrVal",			s->m_parser_string_key_get_string_value );
}

void UnitsyncFunctionLoader::MMOptions(SpringUnitSyncLib *s)
{
    GetLibFuncPtr( s->m_libhandle, "GetMapOptionCount",			s->m_get_map_option_count );
    GetLibFuncPtr( s->m_libhandle, "GetCustomOptionCount",		s->m_get_custom_option_count );
    GetLibFuncPtr( s->m_libhandle, "GetModOptionCount",			s->m_get_mod_option_count );
    GetLibFuncPtr( s->m_libhandle, "GetSkirmishAIOptionCount",  s->m_get_skirmish_ai_option_count );
    GetLibFuncPtr( s->m_libhandle, "GetOptionKey",				s->m_get_option_key );
    GetLibFuncPtr( s->m_libhandle, "GetOptionName",				s->m_get_option_name );
    GetLibFuncPtr( s->m_libhandle, "GetOptionDesc",				s->m_get_option_desc );
    GetLibFuncPtr( s->m_libhandle, "GetOptionType",				s->m_get_option_type );
    GetLibFuncPtr( s->m_libhandle, "GetOptionSection",			s->m_get_option_section );
    GetLibFuncPtr( s->m_libhandle, "GetOptionStyle",			s->m_get_option_style );
    GetLibFuncPtr( s->m_libhandle, "GetOptionBoolDef",			s->m_get_option_bool_def );
    GetLibFuncPtr( s->m_libhandle, "GetOptionNumberDef",		s->m_get_option_number_def );
    GetLibFuncPtr( s->m_libhandle, "GetOptionNumberMin",		s->m_get_option_number_min );
    GetLibFuncPtr( s->m_libhandle, "GetOptionNumberMax",		s->m_get_option_number_max );
    GetLibFuncPtr( s->m_libhandle, "GetOptionNumberStep",		s->m_get_option_number_step );
    GetLibFuncPtr( s->m_libhandle, "GetOptionStringDef",		s->m_get_option_string_def );
    GetLibFuncPtr( s->m_libhandle, "GetOptionStringMaxLen",		s->m_get_option_string_max_len );
    GetLibFuncPtr( s->m_libhandle, "GetOptionListCount",		s->m_get_option_list_count );
    GetLibFuncPtr( s->m_libhandle, "GetOptionListDef",			s->m_get_option_list_def );
    GetLibFuncPtr( s->m_libhandle, "GetOptionListItemKey",		s->m_get_option_list_item_key );
    GetLibFuncPtr( s->m_libhandle, "GetOptionListItemName",		s->m_get_option_list_item_name );
    GetLibFuncPtr( s->m_libhandle, "GetOptionListItemDesc",		s->m_get_option_list_item_desc );
}

void UnitsyncFunctionLoader::Map(SpringUnitSyncLib *s)
{
    GetLibFuncPtr( s->m_libhandle, "GetMapCount",					 s->m_get_map_count );
    GetLibFuncPtr( s->m_libhandle, "GetMapChecksum",				 s->m_get_map_checksum );
    GetLibFuncPtr( s->m_libhandle, "GetMapName",					 s->m_get_map_name );

    try {
        GetLibFuncPtr( s->m_libhandle, "GetMapDescription",             s->m_get_map_description );
        GetLibFuncPtr( s->m_libhandle, "GetMapAuthor",                  s->m_get_map_author );
        GetLibFuncPtr( s->m_libhandle, "GetMapWidth",                   s->m_get_map_width );
        GetLibFuncPtr( s->m_libhandle, "GetMapHeight",                  s->m_get_map_height );
        GetLibFuncPtr( s->m_libhandle, "GetMapTidalStrength",           s->m_get_map_tidalStrength );
        GetLibFuncPtr( s->m_libhandle, "GetMapWindMin",                 s->m_get_map_windMin );
        GetLibFuncPtr( s->m_libhandle, "GetMapWindMax",                 s->m_get_map_windMax );
        GetLibFuncPtr( s->m_libhandle, "GetMapGravity",                 s->m_get_map_gravity );
        GetLibFuncPtr( s->m_libhandle, "GetMapResourceCount",           s->m_get_map_resource_count );
        GetLibFuncPtr( s->m_libhandle, "GetMapResourceName",            s->m_get_map_resource_name );
        GetLibFuncPtr( s->m_libhandle, "GetMapResourceMax",             s->m_get_map_resource_max );
        GetLibFuncPtr( s->m_libhandle, "GetMapResourceExtractorRadius", s->m_get_map_resource_extractorRadius );
        GetLibFuncPtr( s->m_libhandle, "GetMapPosCount",                s->m_get_map_pos_count );
        GetLibFuncPtr( s->m_libhandle, "GetMapPosX",                    s->m_get_map_pos_x );
        GetLibFuncPtr( s->m_libhandle, "GetMapPosZ",                    s->m_get_map_pos_z );
        LslDebug("Using new style map-info fetching (GetMap*() functions).");
    }
    catch ( ... )
    {
        s->m_get_map_name = NULL;
        LslDebug("Using old style map-info fetching (GetMapInfoEx()).");
    }
    GetLibFuncPtr( s->m_libhandle, "GetMapInfoEx",					s->m_get_map_info_ex );
    GetLibFuncPtr( s->m_libhandle, "GetMinimap",					s->m_get_minimap );
    GetLibFuncPtr( s->m_libhandle, "GetInfoMapSize",				s->m_get_infomap_size );
    GetLibFuncPtr( s->m_libhandle, "GetInfoMap",					s->m_get_infomap );

    GetLibFuncPtr( s->m_libhandle, "GetMapArchiveCount",			s->m_get_map_archive_count );
    GetLibFuncPtr( s->m_libhandle, "GetMapArchiveName",             s->m_get_map_archive_name );
    GetLibFuncPtr( s->m_libhandle, "GetMapChecksum",				s->m_get_map_checksum );
    GetLibFuncPtr( s->m_libhandle, "GetMapChecksumFromName",		s->m_get_map_checksum_from_name );
}

void UnitsyncFunctionLoader::Basic(SpringUnitSyncLib *s)
{
    GetLibFuncPtr( s->m_libhandle, "Init",							s->m_init );
    GetLibFuncPtr( s->m_libhandle, "UnInit",						s->m_uninit );
    GetLibFuncPtr( s->m_libhandle, "GetNextError",					s->m_get_next_error );
    GetLibFuncPtr( s->m_libhandle, "GetWritableDataDirectory",		s->m_get_writeable_data_dir );
    GetLibFuncPtr( s->m_libhandle, "GetDataDirectory",				s->m_get_data_dir_by_index );
    GetLibFuncPtr( s->m_libhandle, "GetDataDirectoryCount",         s->m_get_data_dir_count );
    GetLibFuncPtr( s->m_libhandle, "GetSpringVersion",				s->m_get_spring_version );

    GetLibFuncPtr( s->m_libhandle, "AddAllArchives",				s->m_add_all_archives );
    GetLibFuncPtr( s->m_libhandle, "RemoveAllArchives",             s->m_remove_all_archives );

    GetLibFuncPtr( s->m_libhandle, "InitFindVFS",					s->m_init_find_vfs );
    GetLibFuncPtr( s->m_libhandle, "FindFilesVFS",					s->m_find_files_vfs );
    GetLibFuncPtr( s->m_libhandle, "OpenFileVFS",					s->m_open_file_vfs );
    GetLibFuncPtr( s->m_libhandle, "FileSizeVFS",					s->m_file_size_vfs );
    GetLibFuncPtr( s->m_libhandle, "ReadFileVFS",					s->m_read_file_vfs );
    GetLibFuncPtr( s->m_libhandle, "CloseFileVFS",					s->m_close_file_vfs );


    GetLibFuncPtr( s->m_libhandle, "ProcessUnits",					s->m_process_units );
    GetLibFuncPtr( s->m_libhandle, "AddArchive",					s->m_add_archive );
    GetLibFuncPtr( s->m_libhandle, "GetArchiveChecksum",			s->m_get_archive_checksum );
    GetLibFuncPtr( s->m_libhandle, "GetArchivePath",				s->m_get_archive_path );

    GetLibFuncPtr( s->m_libhandle, "OpenArchive",					s->m_open_archive );
    GetLibFuncPtr( s->m_libhandle, "CloseArchive",					s->m_close_archive );
    GetLibFuncPtr( s->m_libhandle, "FindFilesArchive",				s->m_find_Files_archive );
    GetLibFuncPtr( s->m_libhandle, "OpenArchiveFile",				s->m_open_archive_file );
    GetLibFuncPtr( s->m_libhandle, "ReadArchiveFile",				s->m_read_archive_file );
    GetLibFuncPtr( s->m_libhandle, "CloseArchiveFile",				s->m_close_archive_file );
    GetLibFuncPtr( s->m_libhandle, "SizeArchiveFile",               s->m_size_archive_file );

    GetLibFuncPtr( s->m_libhandle, "GetSkirmishAICount",			s->m_get_skirmish_ai_count );
    GetLibFuncPtr( s->m_libhandle, "GetSkirmishAIInfoCount",		s->m_get_skirmish_ai_info_count );
    GetLibFuncPtr( s->m_libhandle, "GetInfoKey",					s->m_get_skirmish_ai_info_key );
    GetLibFuncPtr( s->m_libhandle, "GetInfoValue",					s->m_get_skirmish_ai_info_value );
    GetLibFuncPtr( s->m_libhandle, "GetInfoDescription",			s->m_get_skirmish_ai_info_description );

    GetLibFuncPtr( s->m_libhandle, "GetLuaAICount",                 s->m_get_luaai_count );
    GetLibFuncPtr( s->m_libhandle, "GetLuaAIName",					s->m_get_luaai_name );
    GetLibFuncPtr( s->m_libhandle, "GetLuaAIDesc",					s->m_get_luaai_desc );
}

void UnitsyncFunctionLoader::Config(SpringUnitSyncLib *s)
{
    GetLibFuncPtr( s->m_libhandle, "SetSpringConfigFile",			s->m_set_spring_config_file_path );
    GetLibFuncPtr( s->m_libhandle, "GetSpringConfigFile",           s->m_get_spring_config_file_path );

    GetLibFuncPtr( s->m_libhandle, "SetSpringConfigFloat",			s->m_set_spring_config_float );
    GetLibFuncPtr( s->m_libhandle, "GetSpringConfigFloat",			s->m_get_spring_config_float );
    GetLibFuncPtr( s->m_libhandle, "GetSpringConfigInt",            s->m_get_spring_config_int );
    GetLibFuncPtr( s->m_libhandle, "GetSpringConfigString",         s->m_get_spring_config_string );
    GetLibFuncPtr( s->m_libhandle, "SetSpringConfigString",         s->m_set_spring_config_string );
    GetLibFuncPtr( s->m_libhandle, "SetSpringConfigInt",            s->m_set_spring_config_int );
}

void UnitsyncFunctionLoader::Mod(SpringUnitSyncLib *s)
{
    GetLibFuncPtr( s->m_libhandle, "GetPrimaryModChecksum",         s->m_get_mod_checksum );
    GetLibFuncPtr( s->m_libhandle, "GetPrimaryModIndex",			s->m_get_mod_index );
    GetLibFuncPtr( s->m_libhandle, "GetPrimaryModName",             s->m_get_mod_name );
    GetLibFuncPtr( s->m_libhandle, "GetPrimaryModCount",			s->m_get_mod_count );
    GetLibFuncPtr( s->m_libhandle, "GetPrimaryModArchive",			s->m_get_mod_archive );

    GetLibFuncPtr( s->m_libhandle, "GetSideCount",					s->m_get_side_count );
    GetLibFuncPtr( s->m_libhandle, "GetSideName",					s->m_get_side_name );

    GetLibFuncPtr( s->m_libhandle, "GetPrimaryModShortName",		s->m_get_primary_mod_short_name );
    GetLibFuncPtr( s->m_libhandle, "GetPrimaryModVersion",			s->m_get_primary_mod_version );
    GetLibFuncPtr( s->m_libhandle, "GetPrimaryModMutator",			s->m_get_primary_mod_mutator );
    GetLibFuncPtr( s->m_libhandle, "GetPrimaryModGame",             s->m_get_primary_mod_game );
    GetLibFuncPtr( s->m_libhandle, "GetPrimaryModShortGame",		s->m_get_primary_mod_short_game );
    GetLibFuncPtr( s->m_libhandle, "GetPrimaryModDescription",		s->m_get_primary_mod_description );
    GetLibFuncPtr( s->m_libhandle, "GetPrimaryModArchive",			s->m_get_primary_mod_archive );
    GetLibFuncPtr( s->m_libhandle, "GetPrimaryModArchiveCount",     s->m_get_primary_mod_archive_count );
    GetLibFuncPtr( s->m_libhandle, "GetPrimaryModArchiveList",		s->m_get_primary_mod_archive_list );
    GetLibFuncPtr( s->m_libhandle, "GetPrimaryModChecksumFromName", s->m_get_primary_mod_checksum_from_name );

    GetLibFuncPtr( s->m_libhandle, "GetModValidMapCount",			s->m_get_mod_valid_map_count );
    GetLibFuncPtr( s->m_libhandle, "GetModValidMap",				s->m_get_valid_map );

    GetLibFuncPtr( s->m_libhandle, "GetUnitCount",					s->m_get_unit_count );
    GetLibFuncPtr( s->m_libhandle, "GetUnitName",					s->m_get_unit_name );
    GetLibFuncPtr( s->m_libhandle, "GetFullUnitName",				s->m_get_unit_full_name );
    GetLibFuncPtr( s->m_libhandle, "ProcessUnitsNoChecksum",		s->m_proc_units_nocheck );
}



} // namespace LSL
