cmake_minimum_required(VERSION 3.10)
project(glfwpp)


include(getGLFW.cmake)

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

target_link_libraries(glfwpp glfw3)

add_subdirectory(test)
