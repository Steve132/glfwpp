set(GLFW3_TRY_FETCH YES CACHE BOOL "Try to download GLFW3 as an externalproject if you can't find it")
set(GLFW3_FETCH_VERSION "3.3.2" CACHE STRING "The version of GLFW3 to fetch")
set(GLFW3_SOURCE_DIR "" CACHE PATH "The location of the source code distro for GLFW")
set(GLFW3_INCLUDE_DIR ""  CACHE PATH "The location of glfw.h")
set(GLFW3_LIBRARIES ""  CACHE FILEPATH "The location of glfw.lib")

set(_GLFW_EXTRA_SEARCH_PATH "")

macro(_RUN_GLFW_SEARCH)

if(TARGET glfw3)
set(GLFW3_FOUND True)
else()
set(GLFW3_FOUND False)
endif()

if((NOT GLFW3_FOUND) AND FALSE)
	find_package(GLFW3 QUIET PATHS ${_GLFW_EXTRA_SEARCH_PATH})
	if(GLFW3_FOUND)
		add_library(glfw3 ALIAS GLFW3::GLFW3)
	endif()
endif()

if(NOT GLFW3_FOUND)
	find_package(glfw3 QUIET PATHS ${_GLFW_EXTRA_SEARCH_PATH})
	if(glfw3_FOUND)
		set_target_properties(glfw PROPERTIES IMPORTED_GLOBAL TRUE)
		add_library(glfw3 ALIAS glfw)
		set(GLFW3_FOUND True)
	endif()
endif()

if((NOT GLFW3_FOUND) AND (EXISTS ${GLFW3_INCLUDE_DIR}/glfw3.h) AND (EXISTS ${GLFW3_LIBRARIES}))
	add_library(glfw3 UNKNOWN IMPORTED GLOBAL)
	target_include_directories(glfw3 PUBLIC ${GLFW3_INCLUDE_DIR})
	set_property(TARGET glfw3 IMPORTED_LOCATION ${GLFW3_LIBRARIES})
	set(GLFW3_FOUND True)
endif()

endmacro()

_RUN_GLFW_SEARCH()

if(NOT GLFW3_FOUND)
	
if((NOT EXISTS ${GLFW3_SOURCE_DIR}) AND GLFW3_TRY_FETCH)
	include(FetchContent)
	set(GLFW3_ARCHIVE_URL "https://github.com/glfw/glfw/releases/download/${GLFW3_FETCH_VERSION}/glfw-${GLFW3_FETCH_VERSION}.zip")
	FetchContent_Declare(glfw3 
				URL ${GLFW3_ARCHIVE_URL}
				)
	FetchContent_GetProperties(glfw3)
	if(NOT glfw3_POPULATED)
	  message(STATUS "Populating ${GLFW3_ARCHIVE_URL}")
		FetchContent_Populate(glfw3)
		if(glfw3_POPULATED)
			set(GLFW3_SOURCE_DIR ${glfw3_SOURCE_DIR})
		endif()
	endif()
endif()

if(EXISTS ${GLFW3_SOURCE_DIR})
	include(ExternalProject)
	
	#-DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON #this is broken.  It actually disables the patch and enables the default. it looks like it's copied from bullet.
	# CMAKE_MSVC_RUNTIME_LIBARY=${CMAKE_MSVC_RUNTIME_LIBRARY} #pass through the default
	#-DCMAKE_POLICY_DEFAULT_CMP<NNNN>=<OLD|NEW> set the policy to read the variable.
	set(GLFW_LIB ${CMAKE_CURRENT_BINARY_DIR}/glfw3-external-prefix/install/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX})
	ExternalProject_Add(glfw3-external 
		SOURCE_DIR ${GLFW3_SOURCE_DIR}
		EXCLUDE_FROM_ALL
		CMAKE_ARGS
			-DGLFW_BUILD_DOCS=OFF
			-DGLFW_BUILD_EXAMPLES=OFF
			-DGLFW_BUILD_TESTS=OFF
			-DGLFW_INSTALL=ON
			-DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/glfw3-external-prefix/install
			-DBUILD_SHARED_LIBS=OFF
			-DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON
			-DCMAKE_MSVC_RUNTIME_LIBRARY=${CMAKE_MSVC_RUNTIME_LIBRARY}
			-DCMAKE_POLICY_DEFAULT_CMP0091=NEW
   			-DCMAKE_MAKE_PROGRAM:FILEPATH=${CMAKE_MAKE_PROGRAM}
		STEP_TARGETS install
		INSTALL_BYPRODUCTS ${GLFW_LIB} 
		
	)
	add_library(glfw3 UNKNOWN IMPORTED GLOBAL)
	add_dependencies(glfw3 glfw3-external-install)
	target_include_directories(glfw3 INTERFACE "${GLFW3_SOURCE_DIR}/include")
	set_property(TARGET glfw3 PROPERTY IMPORTED_LOCATION ${GLFW_LIB})
	set(GLFW3_FOUND True)
	
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
