#
# TextureCacheTool CMakeLists.txt
#
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

set(SOURCES
    UserInterface/MainWindow.cpp
    UserInterface/MainWindow.ui
    main.cpp
)

add_executable(TextureCacheTool ${SOURCES})

target_include_directories(TextureCacheTool PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(TextureCacheTool GLideNHQ Qt5::Widgets)
