add_executable(dynamicplot
    dynamicplot.cpp
    ${RC_ICON}
)

target_link_libraries(dynamicplot PRIVATE
    qwtplot3d
    Qt${QT_VERSION_MAJOR}::Widgets
)
