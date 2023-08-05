cmake_minimum_required(VERSION 3.12)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(WebVideoCapture LANGUAGES C CXX)

add_executable(WebVideoCapture "src/web-video-capture.cpp")

set_property(TARGET WebVideoCapture PROPERTY CXX_STANDARD 20)

add_subdirectory(deps)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions("DEBUG")
endif()