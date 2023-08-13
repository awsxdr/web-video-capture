#include "chrome-debug-connector.hpp"
#include "curl-wrapper.hpp"
#include "exceptions.hpp"
#include "logger.hpp"

#include <fmt/core.h>
#include <nlohmann/json.hpp>

#include <chrono>
#include <memory>
#include <string>
#include <thread>

using namespace std;
using namespace web_video_capture;
using namespace fmt;

using json = nlohmann::json;

string get_debug_info_json(int port);
int write_curl_buffer(const char* data, size_t size, size_t member_count, string* buffer);
websocketpp::log::level get_socket_level_for_logging_level(log_levels log_level);

chrome_debug_connector::chrome_debug_connector(int port)
{
	auto const debug_info_json = get_debug_info_json(port);
	auto const debug_info = json::parse(debug_info_json);
	auto const web_socket_debug_url = debug_info[0]["webSocketDebuggerUrl"].get<string>();

	socket_ = make_unique<websocket_client>();

	configure_socket(web_socket_debug_url);

	is_open_ = false;

	error_code error_code;
	connection_ = socket_->get_connection(web_socket_debug_url, error_code);

	socket_->connect(connection_);

	listen_thread_ = thread([this]()
		{
			socket_->run();
		});

	while(!is_open_)
	{
		this_thread::sleep_for(chrono::milliseconds(10));
	}
}

void chrome_debug_connector::configure_socket(const std::string& web_socket_url)
{
	socket_->clear_access_channels(websocketpp::log::alevel::all);
	socket_->set_access_channels(get_socket_level_for_logging_level(logger::get_log_level()));

	socket_->init_asio();

	socket_->set_message_handler([this](auto&& handle, auto&& message) { handle_message(handle, message); });
	socket_->set_open_handler([this](auto&& handle) { handle_socket_open(handle); });
}

void chrome_debug_connector::handle_message(const websocketpp::connection_hdl connection, const message_ptr& message)
{
	
}

void chrome_debug_connector::handle_socket_open(const websocketpp::connection_hdl connection)
{
	is_open_ = true;
}


string get_debug_info_json(int port)
{
	auto const debug_info_address = fmt::format("http://localhost:{}/json", port);

	auto const curl = ::curl();

	auto try_count = 0;
	bool has_connection;
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

	return response_buffer;
}

int write_curl_buffer(const char* data, const size_t size, const size_t member_count, string* buffer)
{
	if (buffer == nullptr)
		return 0;

	buffer->append(data, size * member_count);

	return size * member_count;
}

websocketpp::log::level get_socket_level_for_logging_level(const log_levels log_level)
{
	switch (log_level)
	{
	case log_levels::fatal:
	case log_levels::error:
	case log_levels::warn:
		return websocketpp::log::alevel::fail;

	case log_levels::info:
		return
			websocketpp::log::alevel::fail
			| websocketpp::log::alevel::connect
			| websocketpp::log::alevel::disconnect;

	case log_levels::debug:
		return
			websocketpp::log::alevel::fail
			| websocketpp::log::alevel::connect
			| websocketpp::log::alevel::disconnect
			| websocketpp::log::alevel::debug_close
			| websocketpp::log::alevel::debug_handshake;

	case log_levels::trace:
		return websocketpp::log::alevel::all;
	}

	return websocketpp::log::alevel::none;
}
