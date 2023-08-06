#include "chrome-debug-connector.hpp"

#include <string>
#include <fmt/core.h>

using namespace std;
using namespace web_video_capture;
using namespace fmt;

chrome_debug_connector::chrome_debug_connector(int port)
{
	auto const debug_info_address = format("http://localhost:{}/json", port);


}
