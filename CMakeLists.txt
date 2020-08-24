cmake_minimum_required(VERSION 3.10)
project(glfwpp)

find_package(glfw3)

add_library(glfwpp
	src/GLFW.cpp
	include/GLFW.hpp
	src/Monitor.cpp
	include/Monitor.hpp
	src/Window.cpp
	include/Window.hpp
)

target_compile_features(glfwpp PUBLIC cxx_std_17)
target_include_directories(glfwpp PUBLIC include)

find_package(GLFW3)

if(GLFW3_FOUND)
target_link_libraries(glfwpp GLFW3::GLFW3)
else()
find_package(glfw3)
if(glfw3_FOUND)
message(STATUS "Using glfw3")
target_link_libraries(glfwpp glfw)
endif()
endif()
