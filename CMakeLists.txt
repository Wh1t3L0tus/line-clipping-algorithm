cmake_minimum_required(VERSION 3.6)
project(line_clipping_algorithm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/main.cpp src/shader/Shader.cpp src/shader/Shader.h src/application/Application.cpp src/application/Application.h src/shape/Shape.cpp src/shape/Shape.h src/utility/utils.h src/math/Math.cpp src/math/Math.h)
set(SOURCE_TEST_FILES tests/mainTest.cpp src/math/Math.cpp src/math/Math.h src/utility/utils.h)

add_executable(line_clipping_algorithm ${SOURCE_FILES})
add_executable(line_clipping_algorithm_tests ${SOURCE_TEST_FILES})

# Find GLUT
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)


# Find  GLU
find_package(GLU REQUIRED)
include_directories(${GLU_INCLUDE_DIRS})
link_directories(${GLU_LIBRARY_DIRS})
add_definitions(${GLU_DEFINITIONS})
if(NOT GLU_FOUND)
    message(ERROR " GLU not found!")
endif(NOT GLU_FOUND)

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})
if(NOT GLEW_FOUND)
    message(ERROR " GLEW not found!")
endif(NOT GLEW_FOUND)

#  Find OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

target_link_libraries(line_clipping_algorithm ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLU_LIBRARY} ${GLEW_LIBRARY})