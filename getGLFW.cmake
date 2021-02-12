find_package(GLFW3 QUIET)

if(GLFW3_FOUND)	
	add_library(glfw3 ALIAS GLFW3::GLFW3)
else()
	find_package(glfw3 QUIET)
	if(glfw3_FOUND)
		message(STATUS "Using glfw3")
			add_library(glfw3 ALIAS glfw)
	else()
		set(GLFW3_TRY_FETCH YES CACHE BOOL "Try to download GLFW3 as an externalproject")
		set(GLFW3_FETCH_VERSION "3.3.2" CACHE STRING "The version of GLFW3 to fetch")
		if(GLFW3_TRY_FETCH)
			include(FetchContent)
			FetchContent_Declare(glfw3 
				URL "https://github.com/glfw/glfw/releases/download/${GLFW3_FETCH_VERSION}/glfw-${GLFW3_FETCH_VERSION}.zip"
				)
			set(GLFW_BUILD_DOCS OFF)
			set(GLFW_BUILD_EXAMPLES OFF)
			set(GLFW_BUILD_TESTS OFF) #-DGLFW_INSTALL=YES
			FetchContent_MakeAvailable(glfw3)
			add_library(glfw3 ALIAS glfw)
		else()
			set(GLFW3_INCLUDE_DIR NONE CACHE PATH "The location of glfw.h")
			set(GLFW3_LIB_FILE NONE CACHE FILEPATH "The location of glfw.lib")
			if(EXISTS ${GLFW3_INCLUDE_DIR} AND EXISTS ${GLFW3_LIB_FILE})
				add_library(glfw3 UNKNOWN IMPORTED GLOBAL)
				message(ERROR "This part is probably broken...")
				target_include_directories(glfw3 ${GLFW3_INCLUDE_DIR})
			else()
				message(SEND_ERROR "Cannot build glfwpp, GLFW not found.")
			endif()
		endif()
	endif()
endif()


#include(ExternalProject)
#			ExternalProject_Add(e-glfw3
#				URL "https://github.com/glfw/glfw/releases/download/${GLFW3_FETCH_VERSION}/glfw-${GLFW3_FETCH_VERSION}.zip"
#				CMAKE_ARGS -DGLFW_BUILD_DOCS=NO -DGLFW_BUILD_EXAMPLES=NO -DGLFW_BUILD_TESTS=NO -DGLFW_INSTALL=YES 
#				INSTALL_COMMAND ""
#				)
#			ExternalProject_Get_Property(e-glfw3 SOURCE_DIR)
#			ExternalProject_Get_Property(e-glfw3 BINARY_DIR)
#			find_library 
#			add_library(glfw3 UNKNOWN IMPORTED "${BINARY_DIR}/src/$<CONFIG>/
#			target_include_directories(e-glfw3 INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/glfw3-prefix/src/glfw3/include")