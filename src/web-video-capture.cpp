#include "logger.hpp"
#include "exceptions.hpp"
#include "chrome-finder.hpp"

#include <cxxopts.hpp>
#include <process.hpp>

#include <filesystem>
#include <functional>
#include <string>
#include <tuple>

using namespace std;
using namespace web_video_capture;
using namespace TinyProcessLib;

const string default_log_level = "error";
const string default_capture_time = "0";

typedef tuple<string, int, log_levels> command_line_parameters;

command_line_parameters parse_options(const int argument_count, const char** arguments);

int main(const int argument_count, const char** arguments)
{
	//auto const [url, capture_time, log_level] = parse_options(argument_count, arguments);
	auto const [url, capture_time, log_level] = make_tuple(string("https://news.bbc.co.uk"), 0, web_video_capture::log_levels::trace);

	auto const temp_chrome_profile_path = filesystem::temp_directory_path() / "web-video-capture-chrome-profile";

	auto chrome_process = Process(
		chrome_finder::get_chrome_path() + " --remote-debugging-port=9223 --headless=chrome --disable-gpu --user-data-dir=" + temp_chrome_profile_path.string() + " " + url,
		"");

	Process::kill(chrome_process.get_id());

	filesystem::remove_all(temp_chrome_profile_path);

	return 0;
}

command_line_parameters parse_options(const int argument_count, const char** arguments)
{
	auto options = cxxopts::Options("DerbyStats");

	options.add_options()
		("u,url", "URL to capture", cxxopts::value<std::string>())
		("t,recordtime", "Length of time to capture in milliseconds", cxxopts::value<int>()->default_value(default_capture_time))
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
		log_level);
}