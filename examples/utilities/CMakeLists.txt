
find_package(Qt6 QUIET
    COMPONENTS OpenGLWidgets
)
if(Qt6_FOUND)
    set( QT_VERSION_MAJOR 6 )
else()
    find_package(Qt5 REQUIRED
        COMPONENTS OpenGL
    )
    set(QT_VERSION_MAJOR 5)
endif()


add_library(qwtplot3dExamplesUtilities
    hasOpenGL.cpp
)

target_include_directories(qwtplot3dExamplesUtilities INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)


if(QT_VERSION_MAJOR VERSION_EQUAL 5)
    target_link_libraries(qwtplot3dExamplesUtilities
        PRIVATE
            Qt${QT_VERSION_MAJOR}::OpenGL
    )
else()
    target_link_libraries(qwtplot3dExamplesUtilities
        PRIVATE
            Qt${QT_VERSION_MAJOR}::OpenGLWidgets
    )
endif()