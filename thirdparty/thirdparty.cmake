# OpenGL
find_package(OpenGL REQUIRED)
set(OPENGL_LIBRARY ${OPENGL_LIBRARIES})

# assimp
find_library(ASSIMP_LIBRARY "assimp" "/usr/lib" "/usr/local/lib")
find_path(ASSIMP_INCLUDE_DIR "assimp/mesh.h" "/usr/include" "/usr/local/include")

if((NOT ASSIMP_LIBRARY) OR (NOT ASSIMP_INCLUDE_DIR))
	set(ASSIMP_DIR "${THIRDPARTY_DIR}/assimp")
	
	message("Unable to find assimp, cloning...")
    execute_process(COMMAND git submodule update --init ${ASSIMP_DIR}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

	set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Build package with shared libraries.")
	set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "If the supplementary tools for Assimp are built in addition to the library.")
	set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "If the test suite for Assimp is built in addition to the library.")

    add_subdirectory("${ASSIMP_DIR}")

	set(ASSIMP_LIBRARY "assimp")
	set(ASSIMP_INCLUDE_DIR "${ASSIMP_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/thirdparty/assimp/include")
endif()

set(CMAKE_DEBUG_POSTFIX "")

# glfw
find_library(GLFW_LIBRARY "glfw" "/usr/lib" "/usr/local/lib")
find_path(GLFW_INCLUDE_DIR "glfw/glfw.h" "/usr/include" "/usr/local/include")

if((NOT GLFW_LIBRARY) OR (NOT GLFW_INCLUDE_DIR))
	set(GLFW_DIR "${THIRDPARTY_DIR}/glfw")

	message("Unable to find glfw, cloning...")
    execute_process(COMMAND git submodule update --init ${GLFW_DIR}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

	set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
	set(GLFW_BUILD_TESTS    OFF CACHE INTERNAL "Build the GLFW test programs")
	set(GLFW_BUILD_DOCS     OFF CACHE INTERNAL "Build the GLFW documentation")
	set(GLFW_INSTALL        OFF CACHE INTERNAL "Generate installation target")

    add_subdirectory("${GLFW_DIR}")

	set(GLFW_LIBRARY "glfw" "${GLFW_LIBRARIES}")
	set(GLFW_INCLUDE_DIR "${GLFW_DIR}/include")
endif()

# glad
set(GLAD_DIR "${THIRDPARTY_DIR}/glad")
add_library("glad" STATIC "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")

set(GLAD_LIBRARY "glad")
set(GLAD_INCLUDE_DIR "${GLAD_DIR}/include")

# glm
set(GLM_DIR "${THIRDPARTY_DIR}/glm")
execute_process(COMMAND git submodule update --init ${GLM_DIR}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(GLM_INCLUDE_DIR "${GLM_DIR}")

# stb_image
set(STB_IMAGE_DIR "${THIRDPARTY_DIR}/stb_image")
add_library("stb_image" STATIC "${STB_IMAGE_DIR}/stb_image.cpp")
target_include_directories("stb_image" PRIVATE "${STB_IMAGE_DIR}")

set(STB_IMAGE_LIBRARY "stb_image")
set(STB_IMAGE_INCLUDE_DIR "${STB_IMAGE_DIR}")

# freetype
find_library(FREETYPE_LIBRARY "freetype" "/usr/lib" "/usr/local/lib")
find_path(FREETYPE_INCLUDE_DIR "ft2build.h" "/usr/include" "/usr/local/include")

if((NOT FREETYPE_LIBRARY) OR (NOT FREETYPE_INCLUDE_DIR))
	set(FREETYPE_DIR "${THIRDPARTY_DIR}/freetype")

	message("Unable to find freetype, cloning...")
    execute_process(COMMAND git submodule update --init ${FREETYPE_DIR}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    add_subdirectory("${FREETYPE_DIR}")

	set(FREETYPE_LIBRARY "freetype" "${FREETYPE_LIBRARIES}")
	set(FREETYPE_INCLUDE_DIR "${FREETYPE_DIR}/include")
endif()

# rapidjson
find_library(RAPIDJSON_LIBRARY "rapidjson" "/usr/lib" "/usr/local/lib")
find_path(RAPIDJSON_INCLUDE_DIR "rapidjson/" "/usr/include" "/usr/local/include")

if((NOT RAPIDJSON_LIBRARY) OR (NOT RAPIDJSON_INCLUDE_DIR))
	set(RAPIDJSON_DIR "${THIRDPARTY_DIR}/rapidjson")

	message("Unable to find RapidJSON, cloning...")
    execute_process(COMMAND git submodule update --init ${RAPIDJSON_DIR}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

	option(RAPIDJSON_BUILD_DOC "Build rapidjson documentation." OFF)
	option(RAPIDJSON_BUILD_EXAMPLES "Build rapidjson examples." OFF)
	option(RAPIDJSON_BUILD_TESTS "Build rapidjson perftests and unittests." OFF)
	option(RAPIDJSON_BUILD_CXX11 "Build rapidjson with C++11" OFF)
	option(RAPIDJSON_BUILD_CXX17 "Build rapidjson with C++17" ON)

    add_subdirectory("${RAPIDJSON_DIR}")

	set(RAPIDJSON_LIBRARY "rapidjson" "${FREETYPE_LIBRARIES}")
	set(RAPIDJSON_INCLUDE_DIR "${RAPIDJSON_DIR}/include")
endif()

# openal-soft
set(OPENAL_SOFT_DIR 			"${THIRDPARTY_DIR}/openal-soft")
set(OPENAL_SOFT_INCLUDE_DIR 	"${OPENAL_SOFT_DIR}/include")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(OPENAL_SOFT_LIBRARY	"${OPENAL_SOFT_DIR}/build/libopenal.so")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(OPENAL_SOFT_LIBRARY	"${OPENAL_SOFT_DIR}/build/OpenAL32.lib")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(OPENAL_SOFT_LIBRARY	"${OPENAL_SOFT_DIR}/build/Debug/OpenAL32.lib")
endif()


if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# flac
	find_library(FLAC_LIBRARY "FLAC" "/usr/lib" "/usr/local/lib")
	find_path(FLAC_INCLUDE_DIR "FLAC/all.h" "/usr/include" "/usr/local/include")

	if((NOT FLAC_LIBRARY) OR (NOT FLAC_INCLUDE_DIR))
		set(FLAC_DIR "${THIRDPARTY_DIR}/flac")

		message("Unable to find flac, cloning...")
		execute_process(COMMAND git submodule update --init ${FLAC_DIR}
						WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

		add_subdirectory("${FLAC_DIR}")

		set(FLAC_LIBRARY "flac" "${FLAC_LIBRARIES}")
		set(FLAC_INCLUDE_DIR "${FLAC_DIR}/include")
	endif()


	# opus
	find_library(OPUS_LIBRARY "OPUS" "/usr/lib" "/usr/local/lib")
	find_path(OPUS_INCLUDE_DIR "opus.h" "/usr/include" "/usr/local/include")

	if((NOT OPUS_LIBRARY) OR (NOT OPUS_INCLUDE_DIR))
		set(OPUS_DIR "${THIRDPARTY_DIR}/opus")

		message("Unable to find opus, cloning...")
		execute_process(COMMAND git submodule update --init ${OPUS_DIR}
						WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

		add_subdirectory("${OPUS_DIR}")

		set(OPUS_LIBRARY "opus" "${OPUS_LIBRARIES}")
		set(OPUS_INCLUDE_DIR "${OPUS_DIR}/include")
	endif()
endif()

# libsndfile
find_library(LIBSNDFILE_LIBRARY "libsndfile" "/usr/lib" "/usr/local/lib")
find_path(LIBSNDFILE_INCLUDE_DIR "sndfile.hh" "/usr/include" "/usr/local/include")

if((NOT LIBSNDFILE_LIBRARY) OR (NOT LIBSNDFILE_INCLUDE_DIR))
	set(LIBSNDFILE_DIR "${THIRDPARTY_DIR}/libsndfile")

	message("Unable to find libsndfile, cloning...")
    execute_process(COMMAND git submodule update --init ${LIBSNDFILE_DIR}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

	option(ENABLE_EXTERNAL_LIBS "Enable FLAC, Vorbis, and Opus codecs" OFF)

    add_subdirectory("${LIBSNDFILE_DIR}")

	set(LIBSNDFILE_LIBRARY "sndfile" "${LIBSNDFILE_LIBRARIES}")
	set(LIBSNDFILE_INCLUDE_DIR "${LIBSNDFILE_DIR}/include")
endif()