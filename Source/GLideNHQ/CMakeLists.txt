#
# GLideNHQ CMakeLists.txt
#
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Gui REQUIRED)

set(SOURCES
    TextureCache.cpp
    convert_file.c
)

add_library(GLideNHQ STATIC ${SOURCES})

target_link_libraries(GLideNHQ Qt5::Core Qt5::Gui z)
