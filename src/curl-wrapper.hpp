#pragma once

#include <curl/curl.h>

namespace web_video_capture
{
	class curl
	{
	private:
		CURL* curl_;

	public:
		curl() : curl_(curl_easy_init())
		{			
		}

		virtual ~curl()
		{
			curl_easy_cleanup(curl_);
		}

		template<typename... TArgs>
		void set_option(CURLoption option, const TArgs& ... arguments) const
		{
			curl_easy_setopt(curl_, option, arguments...);
		}

		CURLcode perform() const
		{
			return curl_easy_perform(curl_);
		}

		template<typename... TArgs>
		CURLcode get_info(CURLINFO info_type, const TArgs& ... arguments) const
		{
			return curl_easy_getinfo(curl_, info_type, arguments...);
		}
	};
}