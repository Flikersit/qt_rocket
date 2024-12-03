cmake_minimum_required(VERSION 3.16)
project(hello_world_qt LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network)

qt_standard_project_setup()

qt_add_executable(hello_world_qt
    main.cpp
    RocketObject.cpp RocketObject.h
    RocketScene.cpp RocketScene.h
    ControlPanel.cpp ControlPanel.h
)

set_target_properties(hello_world_qt PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(hello_world_qt PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
)
