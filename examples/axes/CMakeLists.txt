add_executable(axes
    src/axesmainwindow.cpp
    src/axesmainwindowbase.ui
    src/main.cpp
    ${RC_ICON}
)

target_link_libraries(axes PRIVATE
    qwtplot3dExamplesUtilities
    qwtplot3d
    Qt${QT_VERSION_MAJOR}::Widgets
)
