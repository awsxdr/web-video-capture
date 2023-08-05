cmake_minimum_required (VERSION 3.12)

include(FetchContent)
FetchContent_Declare(
	websocketpp_content
	GIT_REPOSITORY	https://github.com/zaphoyd/websocketpp.git
	GIT_TAG			0.8.2
)
FetchContent_MakeAvailable(websocketpp_content)