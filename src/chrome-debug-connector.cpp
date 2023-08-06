#include "chrome-debug-connector.hpp"
#include "curl-wrapper.hpp"
#include "exceptions.hpp"

#include <string>
#include <fmt/core.h>
#include <iostream>

using namespace std;
using namespace web_video_capture;
using namespace fmt;

string get_debug_info_json(int port);
int write_curl_buffer(const char* data, size_t size, size_t member_count, string* buffer);

chrome_debug_connector::chrome_debug_connector(int port)
{
	auto const debug_info_json = get_debug_info_json(port);
}

string get_debug_info_json(int port)
{
	auto const debug_info_address = format("http://localhost:{}/json", port);

	auto const curl = ::curl();

	string response_buffer;
	curl.set_option(CURLOPT_URL, debug_info_address.c_str());
	curl.set_option(CURLOPT_WRITEFUNCTION, &write_curl_buffer);
	curl.set_option(CURLOPT_WRITEDATA, &response_buffer);

	if (curl.perform() != CURLE_OK)
	{
		throw chrome_connection_failed_exception();
	}

	cout << response_buffer << endl;

	return response_buffer;
}

int write_curl_buffer(const char* data, const size_t size, const size_t member_count, string* buffer)
{
	if (buffer == nullptr)
		return 0;

	buffer->append(data, size * member_count);

	return size * member_count;
}