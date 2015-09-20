/* This file is part of the Springlobby (GPL v2 or later), see COPYING */

#ifndef LSL_FUNCTION_PTR_H
#define LSL_FUNCTION_PTR_H

#include <string>
#include <lslutils/debug.h>
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
	if (handle == NULL)
		return;
#ifdef WIN32
	FreeLibrary((HMODULE)handle);
#else
	dlclose(handle);
#endif
}

void* _LoadLibrary(const std::string& libpath)
{
	void* res = NULL;
#ifdef WIN32
	const std::wstring wparentpath = Util::s2ws(LSL::Util::ParentPath(libpath));
	const std::wstring wlibpath = Util::s2ws(libpath);
	SetDllDirectory(NULL);
	SetDllDirectory(wparentpath.c_str());
	res = LoadLibrary(wlibpath.c_str());
	if (res == NULL) {
		const std::string errmsg = Util::geterrormsg().c_str();
		LSL_THROWF(unitsync, "Couldn't load the unitsync library: %s", errmsg.c_str());
	}
#else
	res = dlopen(libpath.c_str(), RTLD_GLOBAL | RTLD_LAZY);
	if (res == NULL) {
		const char* errmsg = dlerror();
		LSL_THROWF(unitsync, "Couldn't load the unitsync library '%s': %s", libpath.c_str(), errmsg);
	}
#endif
	return res;
}

void* GetLibFuncPtr(void* libhandle, const std::string& name)
{
	if (libhandle == NULL)
		return NULL;

#if defined _WIN32
	void* p = (void*)GetProcAddress((HMODULE)libhandle, name.c_str());
#else  // defined _WIN32
	void* p = dlsym(libhandle, name.c_str());
#endif // else defined _WIN32

	if (p == NULL) {
		LslError("Couldn't load %s from unitsync library", name.c_str());
	}
	return p;
}


} // namespace LSL

#endif // LSL_FUNCTION_PTR_H
