#pragma once

#include <iostream>
#include <string>

#include <fmt/core.h>

namespace web_video_capture
{
	using namespace std;
	using namespace fmt;

	enum class log_levels
	{
		fatal,
		error,
		warn,
		info,
		debug,
		trace
	};

	class logger
	{
	private:
		inline static log_levels log_level_ = log_levels::warn;

	public:
		static void set_log_level(log_levels log_level)
		{
			log_level_ = log_level;
		}

		static log_levels get_log_level()
		{
			return log_level_;
		}

		template<class... Types>
		static void log_fatal(const std::string& format, Types... args)
		{
			log(log_levels::fatal, format, args...);
		}

		template<class... Types>
		static void log_error(const std::string& format, Types... args)
		{
			log(log_levels::error, format, args...);
		}

		template<class... Types>
		static void log_warn(const std::string& format, Types... args)
		{
			log(log_levels::warn, format, args...);
		}

		template<class... Types>
		static void log_info(const std::string& format, Types... args)
		{
			log(log_levels::info, format, args...);
		}

		template<class... Types>
		static void log_debug(const std::string& format, Types... args)
		{
			log(log_levels::debug, format, args...);
		}

		template<class... Types>
		static void log_trace(const std::string& format, Types... args)
		{
			log(log_levels::trace, format, args...);
		}

	private:
		template<class... Types>
		static void log(const log_levels log_level, const std::string_view message, Types&&... args)
		{
			if (log_level_ < log_level) return;

			auto const formatted_message = vformat(message, make_format_args(args...));

			cout << formatted_message << endl;
		}
	};
}