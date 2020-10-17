cmake_minimum_required(VERSION 3.16)

project(ft_vox VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

file(GLOB SOURCES
    "${PROJECT_SOURCE_DIR}/src/ft_vox.cpp"
    "${PROJECT_SOURCE_DIR}/src/Camera.cpp"
    "${PROJECT_SOURCE_DIR}/src/Inputs.cpp"
    "${PROJECT_SOURCE_DIR}/src/Mesh.cpp"
    "${PROJECT_SOURCE_DIR}/src/Time.cpp"
	"${PROJECT_SOURCE_DIR}/src/Render.cpp"
)

add_executable(ft_vox ${SOURCES})
target_link_libraries(ft_vox
	${SDL2_LIBRARIES}
	${SDL2_IMAGE_LIBRARIES}
	${SDL2_TTF_LIBRARIES}
	${GLM_LIBRARIES}
	${GLEW_LIBRARIES}
	${OPENGL_LIBRARIES}
)
