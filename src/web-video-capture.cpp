#include "web-video-capture.hpp"

#include "logger.hpp"
#include "chrome-finder.hpp"
#include "chrome-debug-connector.hpp"

#include <cxxopts.hpp>
#include <process.hpp>
#include <fmt/core.h>

#include <chrono>
#include <filesystem>
#include <functional>
#include <string>
#include <tuple>


using namespace std;
using namespace web_video_capture;
using namespace TinyProcessLib;

const string default_log_level = "error";
const string default_capture_time = "0";
const string default_debug_port = "9223";

typedef tuple<string, int, int, log_levels> command_line_parameters;

command_line_parameters parse_options(const int argument_count, const char** arguments);

int main(const int argument_count, const char** arguments)
{
	//auto const [url, capture_time, log_level] = parse_options(argument_count, arguments);
	auto const [url, capture_time, debug_port, log_level] = make_tuple(string("https://news.bbc.co.uk"), 0, 9223, web_video_capture::log_levels::trace);

	auto const temp_chrome_profile_path = filesystem::temp_directory_path() / "web-video-capture-chrome-profile";

	auto chrome_process = Process(
		fmt::format(
			"{} --remote-debugging-port={} --headless=chrome --disable-gpu --user-data-dir={} {}",
			chrome_finder::get_chrome_path(),
			debug_port,
			temp_chrome_profile_path.string(),
			url
		),
		"");

	auto debug_connector = chrome_debug_connector(debug_port);
	debug_connector.start_capture();

	auto const end_time = chrono::steady_clock::now() + chrono::seconds(10);

	while(chrono::steady_clock::now() < end_time)
	{
		this_thread::sleep_for(chrono::milliseconds(10));
	}

	Process::kill(chrome_process.get_id());

	int exit_code;
	while(!chrome_process.try_get_exit_status(exit_code))
	{		
	}

	//filesystem::remove_all(temp_chrome_profile_path);

	return 0;
}

command_line_parameters parse_options(const int argument_count, const char** arguments)
{
	auto options = cxxopts::Options("DerbyStats");

	options.add_options()
		("u,url", "URL to capture", cxxopts::value<std::string>())
		("t,recordtime", "Length of time to capture in milliseconds", cxxopts::value<int>()->default_value(default_capture_time))
		("p,debugport", "Port to use for communicating with Chromium instance", cxxopts::value<int>()->default_value(default_debug_port))
		("l,loglevel", "Logging level", cxxopts::value<string>()->default_value(default_log_level))
		;

	auto const parsed_options = options.parse(argument_count, arguments);

	auto const log_level_string = parsed_options["loglevel"].as<string>();

	function<log_levels(string)> parse_log_level = [&](const string& value) -> log_levels
	{
		return
			value == "fatal" ? log_levels::fatal
			: value == "error" ? log_levels::error
			: value == "warn" ? log_levels::warn
			: value == "info" ? log_levels::info
			: value == "debug" ? log_levels::debug
			: value == "trace" ? log_levels::trace
			: parse_log_level(default_log_level);
	};

	auto const log_level = parse_log_level(log_level_string);

	return std::make_tuple(
		parsed_options["url"].as<string>(),
		parsed_options["recordtime"].as<int>(),
		parsed_options["debugport"].as<int>(),
		log_level);
}