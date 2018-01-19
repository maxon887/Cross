# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

file(GLOB CROSS_SRC
    "../../../Sources/*.cpp"
    "../../../Sources/Graphics3D/*.cpp"
    "../../../Sources/Graphics3D/Simple/*.cpp"
    "../../../Sources/Graphics3D/Light/*.cpp"
    "../../../Sources/Graphics3D/Maps/*.cpp"
    "../../../Sources/Graphics3D/Misc/*.cpp"
    "../../../Sources/Physics/*.cpp"
    "../../../Sources/ThirdParty/ImGui/*.cpp"
    "../../../Sources/UI/*.cpp"
    "../../../../Sources/Core/*.cpp"
    "../../../../Sources/Math/*.cpp"
    "../../../../Sources/Shaders/*.cpp"
    "../../../../Sources/Internals/*.cpp"
    "../../../../Sources/Utils/*.cpp"
    "../../../../Sources/Libs/SOIL/*.c"
    "../../../../Sources/Libs/TinyXML2/*.cpp"
    "../../../../Sources/Platform/*.cpp"
    "../../../../Sources/Platform/Android/*.cpp"

)

add_library( # Sets the name of the library.
             cross
             # Sets the library as a shared library.
             SHARED
             # Provides a relative path to your source file(s).
             # Associated headers in the same location as their source
             # file are automatically included.
             src/main/jni/main.cpp
             ${CROSS_SRC} )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because system libraries are included in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

add_library( fmod SHARED IMPORTED )
add_library( assimp SHARED IMPORTED )
add_library( freetype2 SHARED IMPORTED )

set_target_properties( fmod PROPERTIES IMPORTED_LOCATION
					  ../../../../src/main/jniLibs/${ANDROID_ABI}/libfmod.so )

set_target_properties( assimp PROPERTIES IMPORTED_LOCATION
				  ../../../../src/main/jniLibs/${ANDROID_ABI}/libassimp.so )

set_target_properties( freetype2 PROPERTIES IMPORTED_LOCATION
			  ../../../../src/main/jniLibs/${ANDROID_ABI}/libfreetype2.so )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
						cross
						${log-lib}
						android
						GLESv2
						EGL
						fmod
						assimp
						#freetype2
						)

include_directories(../../../../Sources)
include_directories(../../../../Sources/Core)
include_directories(../../../Sources)
include_directories(../../../../Sources/Libs)
include_directories(../../../../Sources/Libs/FreeType)
include_directories(../../../../Sources/Libs/FreeType/freetype2)
include_directories(../../../../Sources/Libs/TinyXML)
include_directories(../../../../Sources/Libs/Assimp)
include_directories(../../../../Sources/Libs/SOIL)