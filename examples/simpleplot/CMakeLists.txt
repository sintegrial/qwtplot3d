add_executable(simpleplot
    simpleplot.cpp
    ${RC_ICON}
)

target_link_libraries(simpleplot PRIVATE
    qwtplot3d
    Qt${QT_VERSION_MAJOR}::Widgets
)
