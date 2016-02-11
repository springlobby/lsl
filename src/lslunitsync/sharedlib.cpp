/* This file is part of the Springlobby (GPL v2 or later), see COPYING */

#ifndef LSL_FUNCTION_PTR_H
#define LSL_FUNCTION_PTR_H

#include <string>
#include "lslutils/logging.h"
#ifdef WIN32
#include <windows.h>
#include <lslutils/conversion.h>
#include <lslutils/misc.h>
#else
#include <dlfcn.h>
#endif

namespace LSL
{

void _FreeLibrary(void* handle)
{
	if (handle == nullptr)
		return;
#ifdef WIN32
	FreeLibrary((HMODULE)handle);
#else
	dlclose(handle);
#endif
}

void* _LoadLibrary(const std::string& libpath)
{
	void* res = nullptr;
#ifdef WIN32
	const std::wstring wparentpath = Util::s2ws(LSL::Util::ParentPath(libpath));
	const std::wstring wlibpath = Util::s2ws(libpath);
	SetDllDirectory(nullptr);
	SetDllDirectory(wparentpath.c_str());
	res = LoadLibrary(wlibpath.c_str());
	if (res == nullptr) {
		const std::string errmsg = Util::geterrormsg().c_str();
		LslError("Couldn't load the unitsync library: %s", errmsg.c_str());
		return res;
	}
#else
	static Lmid_t lmid = LM_ID_NEWLM;

	res = dlmopen(lmid, libpath.c_str(), RTLD_LOCAL | RTLD_LAZY);
	if (res == nullptr) {
		const char* errmsg = dlerror();
		LslError("Couldn't load the unitsync library '%s': %s", libpath.c_str(), errmsg);
		return nullptr;
	}
	if (lmid == LM_ID_NEWLM) { //store namespace / lmid for future usage:
		const int ret = dlinfo(res, RTLD_DI_LMID, &lmid);
		if (ret != 0) {
			LslError("dlinfo failed, couln't get lmid!");
		}
	}
#endif
	return res;
}

void* GetLibFuncPtr(void* libhandle, const std::string& name)
{
	if (libhandle == nullptr)
		return nullptr;

#if defined _WIN32
	void* p = (void*)GetProcAddress((HMODULE)libhandle, name.c_str());
#else  // defined _WIN32
	void* p = dlsym(libhandle, name.c_str());
#endif // else defined _WIN32

	if (p == nullptr) {
		LslError("Couldn't load %s from unitsync library", name.c_str());
	}
	return p;
}


} // namespace LSL

#endif // LSL_FUNCTION_PTR_H
