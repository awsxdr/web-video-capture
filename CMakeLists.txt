cmake_minimum_required(VERSION 3.14)

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
	find_package(curl REQUIRED)

	set(copy_cmd xcopy )
	set(copy_post_cmd  /Y)
	set(link_extension .dll)
else()
	set(copy_cmd cp -f )
	set(copy_post_cmd)
	set(link_extension .so)
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
)

if (WIN32)
	target_link_libraries(web-video-capture PUBLIC
		"${curl_name}_imp.lib"
		"${fmt_name}.lib"
	)
else()
	target_link_libraries(web-video-capture PUBLIC
		"${curl_name}.so"
		"lib${fmt_name}.so"
	)
endif()

include_directories(
	websocketpp
	"${cxxopts_SOURCE_DIR}/include"
	"${tiny_process_library_dir}"
	"${json_dir}/include"
	"${fmt_src}/include"
	"${curl_src}/include"
)

if(WIN32)
	set(fmt_dll_path "${fmt_bin}/bin/${fmt_name}${link_extension}")
	string(REPLACE "/" "\\" fmt_dll_path_native "${fmt_dll_path}")
	set(curl_dll_path "${curl_bin}/lib/${curl_name}${link_extension}")
	string(REPLACE "/" "\\" curl_dll_path_native "${curl_dll_path}")
	string(REPLACE "/" "\\" binary_dir_native "${CMAKE_CURRENT_BINARY_DIR}")

	add_custom_command(
		TARGET web-video-capture POST_BUILD
		COMMAND 
			${copy_cmd} ${fmt_dll_path_native} ${binary_dir_native} ${copy_post_cmd} &&
			${copy_cmd} ${curl_dll_path_native} ${binary_dir_native} ${copy_post_cmd}
	)
endif()