cmake_minimum_required (VERSION 3.0.0)
project (nes-chr-editor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PNG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

file(GLOB srcs src/*.cpp)
file(GLOB editor_srcs src/editor/*.cpp src/editor/ui/*.cpp)

add_executable (nes-chr-editor ${srcs} ${editor_srcs})
target_link_libraries(nes-chr-editor ${PNG_LIBRARIES} ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} )
target_include_directories(nes-chr-editor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src ${PNG_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
