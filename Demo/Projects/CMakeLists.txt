cmake_minimum_required(VERSION 3.9.1)

project(Demo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB SRC
    ../../Sources/Core/*.*
    ../../Sources/Math/*.*
    ../../Sources/Shaders/*.*
	../../Sources/Internals/*.*
	../../Sources/Experimental/*.*
    ../../Sources/Utils/*.*
    ../../Sources/Platform/MacOS/*.*
    ../../Sources/Libs/SOIL/*.*
    ../../Sources/Libs/TinyXML2/*.*

    ../Sources/*.*
    ../Sources/Graphics3D/*.*
    ../Sources/Graphics3D/Simple/*.*
    ../Sources/Graphics3D/Light/*.*
    ../Sources/Graphics3D/Maps/*.*
    ../Sources/Graphics3D/Misc/*.*
    ../Sources/ThirdParty/ImGui/*.*
    ../Sources/UI/*.*

    MacOS/main.cpp)

add_executable(${PROJECT_NAME} ${SRC})

find_package (GLEW REQUIRED)
find_package (OpenGL REQUIRED)

target_link_libraries (${PROJECT_NAME}
        ${GLEW_LIBRARIES}
        ${OPENGL_gl_LIBRARY}
        /usr/local/lib/libGLFW.dylib
		"${CMAKE_SOURCE_DIR}/../../Binaries/MacOS/libassimpMAC.dylib"
		"${CMAKE_SOURCE_DIR}/../../Binaries/MacOS/libfmod.dylib" )

include_directories(../Sources)
include_directories(../../Sources)
include_directories(../../Sources/Core)
include_directories(../../Sources/Libs)
include_directories(../../Sources/Libs/FreeType)
include_directories(../../Sources/Libs/FreeType/freetype2)
include_directories(../../Sources/Libs/TinyXML)
include_directories(../../Sources/Libs/Assimp)
include_directories(../../Sources/Libs/SOIL)

add_definitions(-DOPENGL)
add_definitions(-DMACOS)
add_definitions(-DCROSS_DEBUG)

add_custom_command(TARGET Demo POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/../../Binaries/MacOS/libfmod.dylib"
		"${CMAKE_CURRENT_BINARY_DIR}/libfmod.dylib")
add_custom_command(TARGET Demo POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${PROJECT_SOURCE_DIR}/../../Binaries/MacOS/libassimpMAC.dylib"
		"${CMAKE_CURRENT_BINARY_DIR}/libassimpMAC.dylib")

file(COPY "${PROJECT_SOURCE_DIR}/../Assets/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Assets/")
