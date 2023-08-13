cmake_minimum_required (VERSION 3.12)
set_property(TARGET web-video-capture PROPERTY CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
	websocketpp
	GIT_REPOSITORY	https://github.com/zaphoyd/websocketpp.git
	GIT_TAG			0.8.2
)
FetchContent_MakeAvailable(websocketpp)

include_directories("${asio_SOURCE_DIR}/include")

set_property(GLOBAL PROPERTY WEBSOCKETPP_SOURCE_DIR "${websocketpp_SOURCE_DIR}")