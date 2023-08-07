#include "chrome-debug-connector.hpp"
#include "curl-wrapper.hpp"
#include "exceptions.hpp"

#include <fmt/core.h>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

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
	auto const debug_info_address = fmt::format("http://localhost:{}/json", port);

	auto const curl = ::curl();

	auto try_count = 0;
	auto has_connection = false;

	string response_buffer;

	do
	{
		if(try_count > 0)
		{
			this_thread::sleep_for(chrono::seconds(1));
		}

		curl.set_option(CURLOPT_URL, debug_info_address.c_str());
		curl.set_option(CURLOPT_WRITEFUNCTION, &write_curl_buffer);
		curl.set_option(CURLOPT_WRITEDATA, &response_buffer);

		has_connection = curl.perform() == CURLE_OK;
	} while (!has_connection && ++try_count < 10);

	if (!has_connection)
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