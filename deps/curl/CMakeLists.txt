include(FetchContent)

set(CURL_STATICLIB ON)

FetchContent_Declare(
	curl_content
	GIT_REPOSITORY	https://github.com/curl/curl.git
	GIT_TAG			curl-8_2_1
)
FetchContent_MakeAvailable(curl_content)

set_property(GLOBAL PROPERTY CURL_SOURCE_DIR "${curl_content_SOURCE_DIR}")
set_property(GLOBAL PROPERTY CURL_BUILD_DIR "${curl_content_BINARY_DIR}")
