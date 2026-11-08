add_executable(volumeplot
    volumeplot.cpp
    ${RC_ICON}
)

target_link_libraries(volumeplot PRIVATE
    qwtplot3d
    Qt${QT_VERSION_MAJOR}::Widgets
)

add_custom_command(TARGET volumeplot POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/../data"
            "$<TARGET_FILE_DIR:volumeplot>/../data/"
    COMMENT "Copying data directory to the build folder for volumeplot."
)
