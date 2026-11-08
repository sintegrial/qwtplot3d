cmake_minimum_required(VERSION 3.1.0)

project(qwtplot3d
    LANGUAGES CXX
)

# Automatically execute the Qt Meta-Object Compiler (moc) when required.
set(CMAKE_AUTOMOC ON)


find_package(Qt6 QUIET
    COMPONENTS Core Gui OpenGLWidgets
)
if(Qt6_FOUND)
    set( QT_VERSION_MAJOR 6 )
else()
    find_package(Qt5 REQUIRED
        COMPONENTS Widgets Core Gui OpenGL
    )
    set(QT_VERSION_MAJOR 5)
endif()


find_package(OpenGL REQUIRED
    COMPONENTS OpenGL
)


file(GLOB SOURCES "src/*.cpp" "include/*.h")
add_library(${PROJECT_NAME}
    ${SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
)

if(QT_VERSION_MAJOR VERSION_EQUAL 5)
    target_link_libraries(${PROJECT_NAME}
        PUBLIC
            Qt${QT_VERSION_MAJOR}::OpenGL
    )
else()
    target_link_libraries(${PROJECT_NAME}
        PUBLIC
            Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    )
endif()

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        OpenGL::GL
        OpenGL::GLU
)

if(MSVC)
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            -fp:fast -arch:SSE2
    )
else()
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            -ffast-math -msse2
    )
endif()

# Only add the examples if this is the top-level CMake project.
if(${CMAKE_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
  add_subdirectory( examples )
endif()
