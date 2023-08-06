#include "chrome-finder.hpp"
#include "exceptions.hpp"

#include <filesystem>
#include <string>

#ifdef WIN32
#include <Windows.h>
#endif

using namespace web_video_capture;
using namespace std;

#ifdef WIN32
string get_chrome_path_from_windows_registry();
#endif

string chrome_finder::get_chrome_path()
{
#ifdef WIN32
	return get_chrome_path_from_windows_registry();
#else
#endif
}

#ifdef WIN32
string get_chrome_path_from_windows_registry()
{
	HKEY chrome_registry_key;
	if (
		RegOpenKeyEx(HKEY_LOCAL_MACHINE, R"(SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\chrome.exe)", 0, KEY_QUERY_VALUE, &chrome_registry_key)
		!= ERROR_SUCCESS)
	{
		throw chrome_registry_key_missing_exception();
	}

	DWORD buffer_size = MAX_PATH;
	auto chrome_path = new TCHAR[buffer_size];
	while (
		RegGetValue(chrome_registry_key, nullptr, nullptr, RRF_RT_REG_SZ, nullptr, chrome_path, &buffer_size)
		== ERROR_MORE_DATA)
	{
		buffer_size *= 2;
		delete[] chrome_path;
		chrome_path = new TCHAR[buffer_size];
	}

	if (!filesystem::exists(chrome_path))
	{
		delete[] chrome_path;
		throw chrome_not_found_exception();
	}

	auto string_chrome_path = string(chrome_path);

	RegCloseKey(chrome_registry_key);
	delete[] chrome_path;

	return string_chrome_path;
}
#endif