cmake_minimum_required(VERSION 3.1.0)
enable_language(CXX)

project(qwtplot3d-zjk)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)

file(GLOB SOURCES "src/*.cpp" "include/*.h")
add_library(${PROJECT_NAME} SHARED ${SOURCES})


target_link_libraries(${PROJECT_NAME} Qt5::Core)
target_link_libraries(${PROJECT_NAME} Qt5::Gui)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)