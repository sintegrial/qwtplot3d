cmake_minimum_required(VERSION 3.1.0)

project(qwtplot3d-zjk
    LANGUAGES CXX
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Automatically execute the Qt Meta-Object Compiler (moc) when required.
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED
    COMPONENTS Widgets Core Gui
)

find_package(OpenGL REQUIRED
    COMPONENTS OpenGL
)


file(GLOB SOURCES "src/*.cpp" "include/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCES})


target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        OpenGL::GL
        OpenGL::GLU
)
