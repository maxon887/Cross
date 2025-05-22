cmake_minimum_required(VERSION 3.10)

project(Demo)

set (CMAKE_CXX_STANDARD 17)

include_directories(../../../Sources)
include_directories(../../../Sources/Core)
include_directories(../../../Sources/Internals)
include_directories(../../../Sources/Libs)
include_directories(../../../Demo/Sources)

file(GLOB_RECURSE CROSS_SRC ../../../Sources/*.*)
list(FILTER CROSS_SRC EXCLUDE REGEX "../../../Sources/Platform/.*" )

file(GLOB_RECURSE DEMO_SRC ../../../Demo/Sources/*.*)
list(FILTER DEMO_SRC EXCLUDE REGEX "../../../Demo/Sources/ThirdParty/ImGui/.*" )

file(GLOB IMGUI_SRC ../../../Demo/Sources/ThirdParty/ImGui/*.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/../../../Sources" FILES ${CROSS_SRC})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/../../../Demo/Sources" FILES ${DEMO_SRC})

add_definitions(-DCROSS_CMAKE)

if (APPLE OR WIN32 OR LINUX)
	find_package(OpenGL REQUIRED)
	include_directories(${OPENGL_INCLUDE_DIR})

	add_definitions(-DOPENGL -DCROSS_DEBUG -DCROSS_MEMORY_PROFILE)

	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Generated")

	add_executable(${PROJECT_NAME} WIN32 ${CROSS_SRC} ${DEMO_SRC} ${IMGUI_SRC})
endif()

if (APPLE)
    include_directories(../../../Sources/Platform/MacOS)
    file(GLOB MACOS_SRC ../../../Sources/Platform/MacOS/*.*)
    target_sources(${PROJECT_NAME} PRIVATE ${MACOS_SRC})
    
    add_definitions(-DMACOS)
    
    find_package(GLEW REQUIRED)
    find_package(GLFW3 REQUIRED)
    find_library(COCOA_LIBRARY Cocoa)
    
    target_link_libraries (${PROJECT_NAME} PRIVATE
        ${OPENGL_gl_LIBRARY}
        glfw
        ${COCOA_LIBRARY}
        GLEW::GLEW
        "${CMAKE_SOURCE_DIR}/../MacOS/Binaries/libfmod.dylib"
		"${CMAKE_SOURCE_DIR}/../MacOS/Binaries/libassimpMAC.dylib")

endif()

if (WIN32)
    include_directories(../../../Sources/Platform/Windows)
    file(GLOB WINDOWS_SRC ../../../Sources/Platform/Windows/*.cpp)
	target_sources(${PROJECT_NAME} PRIVATE ${WINDOWS_SRC})

    add_definitions(-DWIN -DCROSS_MEMORY_PROFILE -D_CRT_SECURE_NO_WARNINGS)

	find_library(GLEW_LIB NAMES glew32
		PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../Windows/Binaries/x64)
	find_library(FMOD_LIB NAMES fmod fmod_vc
		PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../Windows/Binaries/x64)
	find_library(ASSIMP_LIB NAMES assimp
		PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../Windows/Binaries/x64)

	target_link_libraries (${PROJECT_NAME} PRIVATE
		${GLEW_LIB}
		${OPENGL_gl_LIBRARY}
		${FMOD_LIB}
		${ASSIMP_LIB})

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/../Windows/Binaries/x64/assimp.dll"
		"${CMAKE_CURRENT_BINARY_DIR}/../Generated/assimp.dll")

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/../Windows/Binaries/x64/fmod.dll"
		"${CMAKE_CURRENT_BINARY_DIR}/../Generated/fmod.dll")

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/../Windows/Binaries/x64/glew32.dll"
		"${CMAKE_CURRENT_BINARY_DIR}/../Generated/glew32.dll")

endif ()

if(ANDROID)
	include_directories(../../../Sources/Platform/Android)
	file(GLOB ANDROID_SRC 	../../../Sources/Platform/Android/*.*
							../../../Sources/Platform/*.*)

	add_library( cross
	             SHARED
				 ${ANDROID_SRC}
	             ${CROSS_SRC}
	             ${DEMO_SRC}
	             ${IMGUI_SRC} )

	find_library( log-lib log )

	add_library( fmod SHARED IMPORTED )
	add_library( assimp SHARED IMPORTED )
	add_library( freetype2 SHARED IMPORTED )

	set_target_properties( fmod PROPERTIES IMPORTED_LOCATION
						  ../../../../src/main/jniLibs/${ANDROID_ABI}/libfmod.so )

	set_target_properties( assimp PROPERTIES IMPORTED_LOCATION
					  ../../../../src/main/jniLibs/${ANDROID_ABI}/libassimp.so )

	set_target_properties( freetype2 PROPERTIES IMPORTED_LOCATION
				  ../../../../src/main/jniLibs/${ANDROID_ABI}/libfreetype2.so )

	target_link_libraries(  cross
							${log-lib}
							android
							GLESv2
							EGL
							fmod
							assimp )

endif()

if(LINUX)
    add_definitions(-DLINUX)
    
    find_package(GLEW REQUIRED)

	target_link_libraries (${PROJECT_NAME} PRIVATE
			${OPENGL_gl_LIBRARY}
			GLEW::GLEW
			"${CMAKE_SOURCE_DIR}/../Linux/Binaries/x86_64/libfmod.so")
endif()