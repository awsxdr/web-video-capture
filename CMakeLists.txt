cmake_minimum_required(VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(web-video-capture LANGUAGES C CXX)

add_executable(web-video-capture "src/web-video-capture.cpp" "src/exceptions.hpp" "src/chrome-finder.cpp" "src/chrome-debug-connector.cpp" )

set_property(TARGET web-video-capture PROPERTY CXX_STANDARD 20)

add_subdirectory(deps)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions("DEBUG")
endif()

get_property(tiny_process_library_dir GLOBAL PROPERTY TINY_PROCESS_LIBRARY_SOURCE_DIR)
get_property(tiny_process_library_build GLOBAL PROPERTY TINY_PROCESS_LIBRARY_BUILD_DIR)
get_property(fmt_src GLOBAL PROPERTY FMT_SOURCE_DIR)
get_property(fmt_bin GLOBAL PROPERTY FMT_BUILD_DIR)
get_property(curl_src GLOBAL PROPERTY CURL_SOURCE_DIR)
get_property(curl_bin GLOBAL PROPERTY CURL_BUILD_DIR)

target_link_directories(web-video-capture PUBLIC "${tiny_process_library_build}" "${fmt_bin}" "${curl_bin}/lib")

if (WIN32)
	set(copy_cmd xcopy )
	set(copy_post_cmd  /Y)
else()
	set(copy_cmd cp -f )
	set(copy_post_cmd)
endif()


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(fmt_name "fmtd")
	set(curl_name "libcurl-d")
else()
	set(fmt_name "fmt")
	set(curl_name "libcurl")
endif()

target_link_libraries(web-video-capture PUBLIC 
	"tiny-process-library${lib_extension}"
	"${fmt_name}${lib_extension}"
	"${curl_name}_imp${lib_extension}"
)

include_directories(
	websocketpp
	"${cxxopts_SOURCE_DIR}/include"
	"${tiny_process_library_dir}"
	"${json_dir}/include"
	"${fmt_src}/include"
	"${curl_src}/include"
)

set(fmt_dll_path "${fmt_bin}/bin/${fmt_name}.dll")
cmake_path(NATIVE_PATH fmt_dll_path NORMALIZE fmt_dll_path_native)
set(curl_dll_path "${curl_bin}/lib/${curl_name}.dll")
cmake_path(NATIVE_PATH curl_dll_path NORMALIZE curl_dll_path_native)
cmake_path(NATIVE_PATH CMAKE_CURRENT_BINARY_DIR NORMALIZE binary_dir_native)

add_custom_command(
	TARGET web-video-capture POST_BUILD
	COMMAND 
		${copy_cmd} ${fmt_dll_path_native} ${binary_dir_native} ${copy_post_cmd} &&
		${copy_cmd} ${curl_dll_path_native} ${binary_dir_native} ${copy_post_cmd}
)