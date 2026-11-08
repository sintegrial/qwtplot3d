add_executable(enrichments
    src/enrichmentmainwindow.cpp
    src/enrichments.cpp
    src/main.cpp
    ${RC_ICON}
)

target_link_libraries(enrichments PRIVATE
    qwtplot3dExamplesUtilities
    qwtplot3d
    Qt${QT_VERSION_MAJOR}::Widgets
)
