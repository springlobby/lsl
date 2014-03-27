/* This file is part of the Springlobby (GPL v2 or later), see COPYING */

#ifdef WIN32
#include <windows.h>
#include <string>
namespace LSL {
namespace Util {

std::wstring s2ws(const std::string& s)
{
	const size_t slength = s.length() + 1;
	const int len = MultiByteToWideChar(CP_ACP , 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_UTF8, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf, len);
	delete[] buf;
	return r;
}

std::string geterrormsg()
{
	const int code = GetLastError();
	static const int bufsize = 256;
	char lpBuffer[bufsize];
	const int len = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, code, MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), lpBuffer, bufsize-1, NULL);
	return std::string(lpBuffer, len);
}

} // namespace Util
} // namespace LSL
#endif //WIN32

