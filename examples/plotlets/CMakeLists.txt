add_executable(plotlets
    src/configframe.cpp
    src/configframebase.ui
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindowbase.ui
    ${RC_ICON}
)

target_link_libraries(plotlets PRIVATE
    qwtplot3d
    Qt${QT_VERSION_MAJOR}::Widgets
)

include_directories(plotlets PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)
