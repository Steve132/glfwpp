cmake_minimum_required(VERSION 3.10)
project(glfwpp)

add_library(glfwpp
	src/GLFW.cpp
	include/GLFW.hpp
	src/Monitor.cpp
	include/Monitor.hpp
	src/Window.cpp
	include/Window.hpp
	src/General.cpp
	include/General.hpp
	src/Joystick.cpp
	include/Joystick.hpp
	src/Cursor.cpp
	include/Cursor.hpp
)

target_compile_features(glfwpp PUBLIC cxx_std_17)
target_include_directories(glfwpp PUBLIC include)

find_package(GLFW3 QUIET)

if(GLFW3_FOUND)
target_link_libraries(glfwpp GLFW3::GLFW3)
else()
find_package(glfw3 QUIET)
if(glfw3_FOUND)
message(STATUS "Using glfw3")
target_link_libraries(glfwpp glfw)
else()
set(GLFW3_INCLUDE_DIR NONE CACHE PATH "The location of glfw.h")
set(GLFW3_LIB_FILE NONE CACHE FILEPATH "The location of glfw.lib")
if(EXISTS ${GLFW3_INCLUDE_DIR} AND EXISTS ${GLFW3_LIB_FILE})
target_include_directories(glfwpp PUBLIC ${GLFW3_INCLUDE_DIR})
target_link_libraries(glfwpp PUBLIC ${GLFW3_LIB_FILE})
else()
message(SEND_ERROR " Cannot build glfwpp, GLFW not found.")
endif()
endif()
endif()

add_subdirectory(test)
