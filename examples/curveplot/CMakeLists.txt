add_executable(curveplot
    curveplot.cpp
    ${RC_ICON}
)

target_link_libraries(curveplot PRIVATE
    qwtplot3d
    Qt${QT_VERSION_MAJOR}::Widgets
)
