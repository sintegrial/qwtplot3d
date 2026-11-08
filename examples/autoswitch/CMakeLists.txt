add_executable(autoswitch 
    autoswitch.h
    autoswitch.cpp
    ${RC_ICON}
)

target_link_libraries(autoswitch PRIVATE
    qwtplot3d
    Qt${QT_VERSION_MAJOR}::Widgets
)
