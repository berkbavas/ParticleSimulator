cmake_minimum_required(VERSION 3.25.1)

project(ParticleSimulator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "$ENV{Qt6_DIR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(EXTERNAL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/External")
set(QT_IMGUI_DIR "${EXTERNAL_DIR}/qtimgui")

set(LIBS
    debug imguid optimized imgui
    debug qt_imgui_widgetsd optimized qt_imgui_widgets
)

file(GLOB_RECURSE SOURCES Source/*.cpp *.qrc)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets OpenGL Gui OpenGLWidgets)

add_executable(ParticleSimulator ${SOURCES})

target_include_directories(ParticleSimulator PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source" "${QT_IMGUI_DIR}/include")

target_link_directories(ParticleSimulator PRIVATE "${QT_IMGUI_DIR}/lib")

target_link_libraries(ParticleSimulator Qt6::Core Qt6::Widgets Qt6::OpenGL Qt6::Gui Qt6::OpenGLWidgets ${LIBS})

add_custom_command(TARGET ParticleSimulator
    POST_BUILD COMMAND ${CMAKE_COMMAND}
    -E copy_directory
    "${CMAKE_SOURCE_DIR}/Resources/"
    "$<TARGET_FILE_DIR:ParticleSimulator>/Resources/"
)

add_custom_command(TARGET ParticleSimulator
    POST_BUILD COMMAND
    Qt6::windeployqt
    --dir "$<TARGET_FILE_DIR:ParticleSimulator>"
    "$<TARGET_FILE_DIR:ParticleSimulator>/$<TARGET_FILE_NAME:ParticleSimulator>"
)
