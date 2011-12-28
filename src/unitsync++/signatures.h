#ifndef LSL_SIGNATURES_H
#define LSL_SIGNATURES_H

#if WIN32
 #define USYNC_CALL_CONV __stdcall
#elif linux
 #define USYNC_CALL_CONV
#elif defined(__WXMAC__)
 #define USYNC_CALL_CONV
#else
 #warning "defaulting to no usync call conv"
 #define USYNC_CALL_CONV
#endif

#include <boost/function.hpp>

namespace LUS {

struct SpringMapInfo;

/**
 * \defgroup DllPointerTypes Pointer types used with the unitsync library.
 * \TODO move from global namespace
 */
/** @{ */

typedef boost::function< const char* () > GetSpringVersionPtr;

typedef boost::function< int (bool, int) > InitPtr;
typedef boost::function< void () > UnInitPtr;
typedef boost::function< const char* () > GetNextErrorPtr;
typedef boost::function< const char* () > GetWritableDataDirectoryPtr;
typedef boost::function< const char* (int) > GetDataDirectoryPtr;
typedef boost::function< int () > GetDataDirectoryCountPtr;

typedef boost::function< int () > GetMapCountPtr;
typedef boost::function< unsigned int (int) > GetMapChecksumPtr;
typedef boost::function< const char* (int) > GetMapNamePtr;
typedef boost::function< const char* (int) > GetMapDescriptionPtr;
typedef boost::function< const char* (int) > GetMapAuthorPtr;
typedef boost::function< int   (int) > GetMapWidthPtr;
typedef boost::function< int   (int) > GetMapHeightPtr;
typedef boost::function< int   (int) > GetMapTidalStrengthPtr;
typedef boost::function< int   (int) > GetMapWindMinPtr;
typedef boost::function< int   (int) > GetMapWindMaxPtr;
typedef boost::function< int   (int) > GetMapGravityPtr;
typedef boost::function< int   (int) > GetMapResourceCountPtr;
typedef boost::function< const char* (int, int) > GetMapResourceNamePtr;
typedef boost::function< float   (int, int) > GetMapResourceMaxPtr;
typedef boost::function< int   (int, int) > GetMapResourceExtractorRadiusPtr;
typedef boost::function< int   (int) > GetMapPosCountPtr;
typedef boost::function< float   (int, int) > GetMapPosXPtr;
typedef boost::function< float   (int, int) > GetMapPosZPtr;

typedef boost::function< int (const char*, SpringMapInfo*, int) > GetMapInfoExPtr;
typedef boost::function< void* (const char*, int) > GetMinimapPtr;
typedef boost::function< int (const char*, const char*, int*, int*) > GetInfoMapSizePtr;
typedef boost::function< int (const char*, const char*, void*, int) > GetInfoMapPtr;

typedef boost::function< unsigned int (int) > GetPrimaryModChecksumPtr;
typedef boost::function< int (const char*) > GetPrimaryModIndexPtr;
typedef boost::function< const char* (int) > GetPrimaryModNamePtr;
typedef boost::function< int () > GetPrimaryModCountPtr;
typedef boost::function< const char* (int) > GetPrimaryModArchivePtr;

typedef boost::function< int () > GetSideCountPtr;
typedef boost::function< const char* (int) > GetSideNamePtr;

typedef boost::function< void (const char*) > AddAllArchivesPtr;
typedef boost::function< void () > RemoveAllArchivesPtr;

typedef boost::function< const char * (int) > GetFullUnitNamePtr;
typedef boost::function< const char * (int) > GetUnitNamePtr;
typedef boost::function< int () > GetUnitCountPtr;
typedef boost::function< int () > ProcessUnitsNoChecksumPtr;

typedef boost::function< int (const char*) > InitFindVFSPtr;
typedef boost::function< int (int, char*, int) > FindFilesVFSPtr;
typedef boost::function< int (const char*) > OpenFileVFSPtr;
typedef boost::function< int (int) > FileSizeVFSPtr;
typedef boost::function< int (int, void*, int) > ReadFileVFSPtr;
typedef boost::function< void (int) > CloseFileVFSPtr;

typedef boost::function< void (const char*) > SetSpringConfigFilePtr;
typedef boost::function< const char * () > GetSpringConfigFilePtr;

typedef boost::function< int (const char*, int ) > GetSpringConfigIntPtr;
typedef boost::function< const char* (const char*, const char* ) > GetSpringConfigStringPtr;
typedef boost::function< float (const char*, float ) > GetSpringConfigFloatPtr;

typedef boost::function< void (const char*, const char* ) > SetSpringConfigStringPtr;
typedef boost::function< void (const char*, int ) > SetSpringConfigIntPtr;
typedef boost::function< void (const char*, float ) > SetSpringConfigFloatPtr;

typedef boost::function< int (void) > ProcessUnitsPtr;
typedef boost::function< void (const char*) > AddArchivePtr;
typedef boost::function< unsigned int (const char*) > GetArchiveChecksumPtr;
typedef boost::function< const char* (const char*) > GetArchivePathPtr;
typedef boost::function< int (const char*) > GetMapArchiveCountPtr;
typedef boost::function< const char* (int) > GetMapArchiveNamePtr;
typedef boost::function< unsigned int (int) > GetMapChecksumPtr;
typedef boost::function< int (const char*) > GetMapChecksumFromNamePtr;

typedef boost::function< const char* (int) > GetPrimaryModShortNamePtr;
typedef boost::function< const char* (int) > GetPrimaryModVersionPtr;
typedef boost::function< const char* (int) > GetPrimaryModMutatorPtr;
typedef boost::function< const char* (int) > GetPrimaryModGamePtr;
typedef boost::function< const char* (int) > GetPrimaryModShortGamePtr;
typedef boost::function< const char* (int) > GetPrimaryModDescriptionPtr;
typedef boost::function< const char* (int) > GetPrimaryModArchivePtr;
typedef boost::function< int (int) > GetPrimaryModArchiveCountPtr;
typedef boost::function< const char* (int) > GetPrimaryModArchiveListPtr;
typedef boost::function< unsigned int (const char*) > GetPrimaryModChecksumFromNamePtr;
typedef boost::function< unsigned int () > GetModValidMapCountPtr;
typedef boost::function< const char* (int) > GetModValidMapPtr;

typedef boost::function< int () > GetLuaAICountPtr;
typedef boost::function< const char* (int) > GetLuaAINamePtr;
typedef boost::function< const char* (int) > GetLuaAIDescPtr;

typedef boost::function< int (const char*) > GetMapOptionCountPtr;
typedef boost::function< int (const char*) > GetCustomOptionCountPtr;
typedef boost::function< int () > GetModOptionCountPtr;
typedef boost::function< int (int) > GetSkirmishAIOptionCountPtr;
typedef boost::function< const char* (int) > GetOptionKeyPtr;
typedef boost::function< const char* (int) > GetOptionNamePtr;
typedef boost::function< const char* (int) > GetOptionDescPtr;
typedef boost::function< const char* (int) > GetOptionSectionPtr;
typedef boost::function< const char* (int) > GetOptionStylePtr;
typedef boost::function< int (int) > GetOptionTypePtr;
typedef boost::function< int (int) > GetOptionBoolDefPtr;
typedef boost::function< float (int) > GetOptionNumberDefPtr;
typedef boost::function< float (int) > GetOptionNumberMinPtr;
typedef boost::function< float (int) > GetOptionNumberMaxPtr;
typedef boost::function< float (int) > GetOptionNumberStepPtr;
typedef boost::function< const char* (int) > GetOptionStringDefPtr;
typedef boost::function< int (int) > GetOptionStringMaxLenPtr;
typedef boost::function< int (int) > GetOptionListCountPtr;
typedef boost::function< const char* (int) > GetOptionListDefPtr;
typedef boost::function< const char* (int, int) > GetOptionListItemKeyPtr;
typedef boost::function< const char* (int, int) > GetOptionListItemNamePtr;
typedef boost::function< const char* (int, int) > GetOptionListItemDescPtr;

typedef boost::function< int (const char*) > OpenArchivePtr;
typedef boost::function< void (int) > CloseArchivePtr;
typedef boost::function< int (int, int, char*, int*) > FindFilesArchivePtr;
typedef boost::function< int (int, const char*) > OpenArchiveFilePtr;
typedef boost::function< int (int, int, void*, int) > ReadArchiveFilePtr;
typedef boost::function< void (int, int) > CloseArchiveFilePtr;
typedef boost::function< int (int, int) > SizeArchiveFilePtr;

typedef boost::function< int () > GetSkirmishAICountPtr;
typedef boost::function< int (int) > GetSkirmishAIInfoCountPtr;
typedef boost::function< const char* (int) > GetInfoKeyPtr;
typedef boost::function< const char* (int) > GetInfoValuePtr;
typedef boost::function< const char* (int) > GetInfoDescriptionPtr;

/// Unitsync functions wrapping lua parser
typedef boost::function< void () > lpClosePtr;
typedef boost::function< int (const char*, const char*, const char*) > lpOpenFilePtr;
typedef boost::function< int (const char*, const char*) > lpOpenSourcePtr;
typedef boost::function< int () > lpExecutePtr;
typedef boost::function< const char* () > lpErrorLogPtr;

typedef boost::function< void (int, int override) > lpAddTableIntPtr;
typedef boost::function< void (const char*, int override) > lpAddTableStrPtr;
typedef boost::function< void () > lpEndTablePtr;
typedef boost::function< void (int, int) > lpAddIntKeyIntValPtr;
typedef boost::function< void (const char*, int) > lpAddStrKeyIntValPtr;
typedef boost::function< void (int, int) > lpAddIntKeyBoolValPtr;
typedef boost::function< void (const char*, int) > lpAddStrKeyBoolValPtr;
typedef boost::function< void (int, float val) > lpAddIntKeyFloatValPtr;
typedef boost::function< void (const char*, float val) > lpAddStrKeyFloatValPtr;
typedef boost::function< void (int, const char* val) > lpAddIntKeyStrValPtr;
typedef boost::function< void (const char*, const char* val) > lpAddStrKeyStrValPtr;

typedef boost::function< int () > lpRootTablePtr;
typedef boost::function< int (const char* expr) > lpRootTableExprPtr;
typedef boost::function< int (int) > lpSubTableIntPtr;
typedef boost::function< int (const char*) > lpSubTableStrPtr;
typedef boost::function< int (const char* expr) > lpSubTableExprPtr;
typedef boost::function< void () > lpPopTablePtr;

typedef boost::function< int (int) > lpGetKeyExistsIntPtr;
typedef boost::function< int (const char*) > lpGetKeyExistsStrPtr;

typedef boost::function< int (int) > lpGetIntKeyTypePtr;
typedef boost::function< int (const char*) > lpGetStrKeyTypePtr;

typedef boost::function< int () > lpGetIntKeyListCountPtr;
typedef boost::function< int (int) > lpGetIntKeyListEntryPtr;
typedef boost::function< int () > lpGetStrKeyListCountPtr;
typedef boost::function< const char* (int) > lpGetStrKeyListEntryPtr;

typedef boost::function< int (int, int) > lpGetIntKeyIntValPtr;
typedef boost::function< int (const char*, int) > lpGetStrKeyIntValPtr;
typedef boost::function< int (int, int) > lpGetIntKeyBoolValPtr;
typedef boost::function< int (const char*, int) > lpGetStrKeyBoolValPtr;
typedef boost::function< float (int, float) > lpGetIntKeyFloatValPtr;
typedef boost::function< float (const char*, float) > lpGetStrKeyFloatValPtr;
typedef boost::function< const char* (int, const char*) > lpGetIntKeyStrValPtr;
typedef boost::function< const char* (const char*, const char*) > lpGetStrKeyStrValPtr;


/** @} */


}

#endif // LSL_SIGNATURES_H
