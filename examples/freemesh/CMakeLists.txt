add_executable(freemesh
    freemesh.cpp
    ${RC_ICON}
)

target_link_libraries(freemesh PRIVATE
    qwtplot3d
    Qt${QT_VERSION_MAJOR}::Widgets
)
