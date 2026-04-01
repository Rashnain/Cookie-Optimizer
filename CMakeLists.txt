cmake_minimum_required(VERSION 3.16)
project(cookie_opimizer LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

qt_add_executable(cookie_opimizer
    main.cpp
    cookie_optimizer.cpp
    QDoubleSpinBoxCustom.cpp
)

target_link_libraries(cookie_opimizer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
