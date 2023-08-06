#pragma once

#include <string>

namespace web_video_capture
{
	class chrome_finder
	{
	public:
		static std::string get_chrome_path();
	};
}