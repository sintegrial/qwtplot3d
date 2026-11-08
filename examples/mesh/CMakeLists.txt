add_executable(mesh
    src/colormapreader.cpp
    src/lightingdialog.cpp
    src/lightingdialogbase.ui
    src/main.cpp
    src/meshmainwindow.cpp
    src/meshmainwindowbase.ui
    ${RC_ICON}
    ${QRC_IMAGES}
)

target_link_libraries(mesh PRIVATE
    qwtplot3dExamplesUtilities
    qwtplot3d
    Qt${QT_VERSION_MAJOR}::Widgets
)
