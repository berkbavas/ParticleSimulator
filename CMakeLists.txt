cmake_minimum_required(VERSION 3.25.1)

project(ParticleSimulator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "$ENV{Qt6_DIR}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(INCLUDE_DIR Libs/qtimgui/include)

set(LIBS_DIR Libs/qtimgui/lib)

set(LIBS
    debug debug/imgui  
    debug debug/qt_imgui_widgets                
    optimized release/imgui
    optimized release/qt_imgui_widgets
)

file(GLOB_RECURSE SOURCES Source/*.cpp *.qrc)

find_package(Qt6 COMPONENTS Core Widgets OpenGL Gui REQUIRED)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/Source" ${INCLUDE_DIR})

link_directories(${LIBS_DIR})

add_executable(ParticleSimulator ${SOURCES})

target_link_libraries(ParticleSimulator Qt6::Core Qt6::Widgets Qt6::OpenGL ${LIBS})

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
