#pragma once

#include <iostream>
#include <string>
#include <format>

namespace web_video_capture
{
	using namespace std;

	enum class log_level
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
		inline static log_level log_level_ = log_level::warn;

	public:
		static void set_log_level(log_level log_level)
		{
			log_level_ = log_level;
		}

		static log_level get_log_level()
		{
			return log_level_;
		}

		template<class... Types>
		static void log_fatal(const std::string& format, Types... args)
		{
			log(log_level::fatal, format, args...);
		}

		template<class... Types>
		static void log_error(const std::string& format, Types... args)
		{
			log(log_level::error, format, args...);
		}

		template<class... Types>
		static void log_warn(const std::string& format, Types... args)
		{
			log(log_level::warn, format, args...);
		}

		template<class... Types>
		static void log_info(const std::string& format, Types... args)
		{
			log(log_level::info, format, args...);
		}

		template<class... Types>
		static void log_debug(const std::string& format, Types... args)
		{
			log(log_level::debug, format, args...);
		}

		template<class... Types>
		static void log_trace(const std::string& format, Types... args)
		{
			log(log_level::trace, format, args...);
		}

	private:
		template<class... Types>
		static void log(const log_level log_level, const std::string_view message, Types&&... args)
		{
			if (log_level_ < log_level) return;

			auto const formatted_message = vformat(message, make_format_args(args...));

			cout << formatted_message << endl;
		}
	};
}