add_executable(graph
    graph.cpp
    ${RC_ICON}
)

target_link_libraries(graph PRIVATE
    qwtplot3d
    Qt${QT_VERSION_MAJOR}::Widgets
)
