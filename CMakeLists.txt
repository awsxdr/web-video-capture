cmake_minimum_required(VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(web-video-capture LANGUAGES C CXX)

add_executable(web-video-capture "src/web-video-capture.cpp" "src/exceptions.hpp" "src/chrome-finder.cpp" "src/chrome-debug-connector.cpp")

set_property(TARGET web-video-capture PROPERTY CXX_STANDARD 20)

add_subdirectory(deps)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions("DEBUG")
endif()

get_property(tiny_process_library_dir GLOBAL PROPERTY TINY_PROCESS_LIBRARY_SOURCE_DIR)
get_property(tiny_process_library_build GLOBAL PROPERTY TINY_PROCESS_LIBRARY_BUILD_DIR)
get_property(fmt_src GLOBAL PROPERTY FMT_SOURCE_DIR)
get_property(fmt_bin GLOBAL PROPERTY FMT_BUILD_DIR)

target_link_directories(web-video-capture PUBLIC "${tiny_process_library_build}" "${fmt_bin}")
target_link_libraries(web-video-capture PUBLIC "tiny-process-library.lib")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_link_libraries(web-video-capture PUBLIC "fmtd.lib")
else()
	target_link_libraries(web-video-capture PUBLIC "fmt.lib")
endif()

include_directories(
	websocketpp
	"${cxxopts_SOURCE_DIR}/include"
	"${tiny_process_library_dir}"
	"${json_dir}/include"
	"${fmt_src}/include"
)