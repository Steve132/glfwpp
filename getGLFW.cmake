set(GLFW3_TRY_FETCH YES CACHE BOOL "Try to download GLFW3 as an externalproject if you can't find it")
set(GLFW3_FETCH_VERSION "3.3.2" CACHE STRING "The version of GLFW3 to fetch")
set(GLFW3_SOURCE_DIR "" CACHE PATH "The location of the source code distro for GLFW")
set(GLFW3_INCLUDE_DIR ""  CACHE PATH "The location of glfw.h")
set(GLFW3_LIBRARIES ""  CACHE FILEPATH "The location of glfw.lib")

if(TARGET glfw3)
set(GLFW3_FOUND True)
else()
set(GLFW3_FOUND False)
endif()


if(NOT GLFW3_FOUND)
	find_package(GLFW3 QUIET)
	if(GLFW3_FOUND)
		add_library(glfw3 ALIAS GLFW3::GLFW3)
	endif()
endif()

if(NOT GLFW3_FOUND)
	find_package(glfw3 QUIET)
	if(glfw3_FOUND)
		add_library(glfw3 ALIAS glfw)
		set(GLFW3_FOUND True)
	endif()
endif()

if((NOT GLFW3_FOUND) AND (EXISTS ${GLFW3_INCLUDE_DIR}) AND (EXISTS ${GLFW3_LIBRARIES}))
	add_library(glfw3 UNKNOWN IMPORTED GLOBAL)
	target_include_directories(glfw3 PUBLIC ${GLFW3_INCLUDE_DIR})
	set_property(TARGET glfw3 IMPORTED_LOCATION ${GLFW3_LIBRARIES})
	set(GLFW3_FOUND True)
endif()


if(NOT GLFW3_FOUND)
	
if((NOT EXISTS ${GLFW3_SOURCE_DIR}) AND GLFW3_TRY_FETCH)
	include(FetchContent)
	set(GLFW3_ZIP_URL "https://github.com/glfw/glfw/releases/download/${GLFW3_FETCH_VERSION}/glfw-${GLFW3_FETCH_VERSION}.zip")
	FetchContent_Declare(glfw3 
				URL ${GLFW3_ZIP_URL}
				)
	FetchContent_GetProperties(glfw3)
	if(NOT glfw3_POPULATED)
	  message(STATUS "Downloading ${GLFW3_ZIP_URL}")
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
	)
	add_library(glfw3 INTERFACE)
	add_library(glfw3-ex UNKNOWN IMPORTED GLOBAL)
	set_target_properties(glfw3-ex PROPERTIES
		IMPORTED_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/glfw3-external-prefix/install/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX}"
	#	IMPORTED_LOCATION_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/glfw3-external-prefix/src/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX}"
	#	IMPORTED_LOCATION_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/glfw3-external-prefix/src/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX}"
	#	IMPORTED_LOCATION_MINSIZEREL "${CMAKE_CURRENT_BINARY_DIR}/glfw3-external-prefix/src/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX}"
	#	IMPORTED_LOCATION_RELWITHDEBINFO "${CMAKE_CURRENT_BINARY_DIR}/glfw3-external-prefix/src/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX}"
	)
	add_dependencies(glfw3-ex glfw3-external)
	target_include_directories(glfw3 INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/glfw3-external-prefix/install/include")
	target_link_libraries(glfw3 INTERFACE glfw3-ex)
	
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