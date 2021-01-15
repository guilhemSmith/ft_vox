cmake_minimum_required(VERSION 3.16)

project(ft_vox VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR})

if(APPLE)
    set(
		PLATFORM
		"APPLE"
	)
endif()
if(UNIX AND NOT APPLE)
	set(
		PLATFORM
		"LINUX"
	)
endif()
if(WIN32)
	set(
		PLATFORM
		"WINDOWS"
	)
endif()

set(
	RES_PATH
	"\"${CMAKE_BINARY_DIR}/resources\""
)

configure_file(
	OS.hpp.in
	OS.hpp
)

file(
	GLOB
	SOURCES
    "${PROJECT_SOURCE_DIR}/src/ft_vox.cpp"
    "${PROJECT_SOURCE_DIR}/src/Camera.cpp"
    "${PROJECT_SOURCE_DIR}/src/Inputs.cpp"
    "${PROJECT_SOURCE_DIR}/src/Mesh.cpp"
    "${PROJECT_SOURCE_DIR}/src/Noise.cpp"
    "${PROJECT_SOURCE_DIR}/src/Time.cpp"
	"${PROJECT_SOURCE_DIR}/src/Render.cpp"
	"${PROJECT_SOURCE_DIR}/src/Shader.cpp"
	"${PROJECT_SOURCE_DIR}/src/Chunk.cpp"
	"${PROJECT_SOURCE_DIR}/src/Skybox.cpp"
	"${PROJECT_SOURCE_DIR}/src/ChunkManager.cpp"
	"${PROJECT_SOURCE_DIR}/src/World.cpp"
	"${PROJECT_SOURCE_DIR}/src/Cavern.cpp"
	"${PROJECT_SOURCE_DIR}/src/Text.cpp"
	"${PROJECT_SOURCE_DIR}/src/OS.cpp"
)

file(
	COPY
	"${PROJECT_SOURCE_DIR}/textures"
	"${PROJECT_SOURCE_DIR}/fonts"
	"${PROJECT_SOURCE_DIR}/Shaders"
	DESTINATION
	"${CMAKE_BINARY_DIR}/resources"
)

add_executable(ft_vox ${SOURCES})
target_link_libraries(ft_vox
	${SDL2_LIBRARIES}
	${SDL2_IMAGE_LIBRARIES}
	${GLM_LIBRARIES}
	Freetype::Freetype
        GLEW::GLEW
	${OPENGL_LIBRARIES}
)
