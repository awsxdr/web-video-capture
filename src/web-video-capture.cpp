#include "logger.hpp"

#include <cxxopts.hpp>
#include <functional>
#include <string>
#include <tuple>

using namespace std;
using namespace web_video_capture;

const string default_log_level = "error";
const string default_capture_time = "0";

typedef tuple<string, int, log_level> command_line_parameters;

command_line_parameters parse_options(const int argument_count, const char** arguments);

int main(const int argument_count, const char** arguments)
{
	auto const [url, capture_time, log_level] = parse_options(argument_count, arguments);

	cout << url << endl;

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

	function<log_level(string)> parse_log_level = [&](const string& value) -> log_level
	{
		return
			value == "fatal" ? log_level::fatal
			: value == "error" ? log_level::error
			: value == "warn" ? log_level::warn
			: value == "info" ? log_level::info
			: value == "debug" ? log_level::debug
			: value == "trace" ? log_level::trace
			: parse_log_level(default_log_level);
	};

	auto const log_level = parse_log_level(log_level_string);

	return std::make_tuple(
		parsed_options["url"].as<string>(),
		parsed_options["recordtime"].as<int>(),
		log_level);
}