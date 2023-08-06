include(FetchContent)
FetchContent_Declare(
	tiny_process_library_content
	GIT_REPOSITORY	http://gitlab.com/eidheim/tiny-process-library
	GIT_TAG			v2.0.4
)
FetchContent_MakeAvailable(tiny_process_library_content)

set_property(GLOBAL PROPERTY TINY_PROCESS_LIBRARY_SOURCE_DIR "${tiny_process_library_content_SOURCE_DIR}")
set_property(GLOBAL PROPERTY TINY_PROCESS_LIBRARY_BUILD_DIR "${tiny_process_library_content_BINARY_DIR}")
