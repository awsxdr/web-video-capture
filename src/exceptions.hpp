#pragma once

#include <exception>

namespace web_video_capture
{
	using namespace std;

	class chrome_registry_key_missing_exception : public exception {};
	class chrome_not_found_exception : public exception {};
	class chrome_connection_failed_exception : public exception {};
	class websocket_debug_url_not_found_exception : public exception {};
}