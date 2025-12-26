cmake_minimum_required(VERSION 4.1)
project(fileexplorer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(fileexplorer main.cpp
        mainwindow.cpp
        mainwindow.h
        fileexplorer.cpp
        fileexplorer.h
        navigationpanel.cpp
        navigationpanel.h)
target_link_libraries(fileexplorer
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

