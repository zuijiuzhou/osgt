cmake_minimum_required(VERSION 3.20)

project(osgt)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(OpenSceneGraph COMPONENTS osgGA osgUtil osgViewer REQUIRED)
find_package(Qt6 COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets REQUIRED)
qt_standard_project_setup()

FILE(GLOB SRC_FILES src/*.cpp)
FILE(GLOB HEADER_FILES src/*.h)

add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES} ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::OpenGL Qt6::OpenGLWidgets ${OPENSCENEGRAPH_LIBRARIES})

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        $<BUILD_INTERFACE:${OPENSCENEGRAPH_INCLUDE_DIRS}>
)
