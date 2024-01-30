# CMakeList.txt : CMake project for SimpleRtspServer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.14)

project(SimpleServer LANGUAGES C CXX)
message("Project name is " ${PROJECT_NAME})

#include_directories("Network")

if (WIN32)
	message("Configuring for Windows")
	add_definitions(-DWINDOWS_OS)
elseif (UNIX)
	message("Configuring for Linux")
	add_definitions(-DLINUX_OS)
endif()

# Add source to this project's executable.
add_executable (${PROJECT_NAME} SimpleRtspServer.h SimpleRtspServer.cpp)

if (WIN32)
	set(PLATFORM_SOURCES ${CMAKE_SOURCE_DIR}/Network/windows/WinNetwork.cpp)
elseif (UNIX)
	# set(PLATFORM_SOURCES ${CMAKE_SOURCE_DIR}/Network/linux/LinNetwork.cpp)
endif()

target_sources(${PROJECT_NAME} PRIVATE 
	${CMAKE_SOURCE_DIR}/Network/Network.cpp
	${CMAKE_SOURCE_DIR}/RtspCore/RtspCore.cpp
	${PLATFORM_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE 
	${CMAKE_SOURCE_DIR}/Network
	$<$<PLATFORM_ID:Linux>:${CMAKE_SOURCE_DIR}/Network/linux>
	$<$<PLATFORM_ID:Windows>:${CMAKE_SOURCE_DIR}/Network/windows>
	${CMAKE_SOURCE_DIR}/RtspCore
	${CMAKE_SOURCE_DIR}/Utils)

if (WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32.lib)

elseif (UNIX)
	# target_link_libraries()

endif()

# message("Current source dir is: " ${CMAKE_SOURCE_DIR})

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
