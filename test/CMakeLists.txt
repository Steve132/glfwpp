include(CTest)

if(BUILD_TESTING)
add_executable(glfwpp_window
	glfwpp_window.cpp
)


target_link_libraries(glfwpp_window INTERFACE GLEW)
target_compile_features(glfwpp_window PUBLIC cxx_std_11)
target_include_directories(glfwpp_window PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include/")
target_include_directories(glfwpp_window PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../")
target_link_libraries(glfwpp_window PRIVATE glfwpp)

endif()
