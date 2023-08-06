include(FetchContent)
FetchContent_Declare(
	fmt_content
	GIT_REPOSITORY	https://github.com/fmtlib/fmt.git
	GIT_TAG			10.0.0
)
FetchContent_MakeAvailable(fmt_content)

set_property(GLOBAL PROPERTY FMT_SOURCE_DIR "${fmt_content_SOURCE_DIR}")
set_property(GLOBAL PROPERTY FMT_BUILD_DIR "${fmt_content_BINARY_DIR}")

