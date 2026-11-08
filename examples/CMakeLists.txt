set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(RC_ICON "${CMAKE_CURRENT_SOURCE_DIR}/icon.rc")
set(QRC_IMAGES "${CMAKE_CURRENT_SOURCE_DIR}/images.qrc")

add_subdirectory(utilities)

add_subdirectory(autoswitch)
add_subdirectory(axes)
add_subdirectory(curveplot)
add_subdirectory(dynamicplot)
add_subdirectory(enrichments)
add_subdirectory(freemesh)
add_subdirectory(graph)
add_subdirectory(mesh)
add_subdirectory(plotlets)
add_subdirectory(simpleplot)
add_subdirectory(volumeplot)
