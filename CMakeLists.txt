cmake_minimum_required(VERSION 3.21)

project(CppMemoryAlgorithmVisualizer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

add_executable(CppMemoryAlgorithmVisualizer
    src/main.cpp
    src/core/SimulationTypes.h
    src/core/SimulationEngine.h
    src/core/SimulationEngine.cpp
    src/ui/MainWindow.h
    src/ui/MainWindow.cpp
    src/ui/VisualizationCanvas.h
    src/ui/VisualizationCanvas.cpp
    src/algorithms/AlgorithmSimulator.h
    src/algorithms/AlgorithmSimulator.cpp
    src/memory/MemorySimulator.h
    src/memory/MemorySimulator.cpp
    src/concurrency/ConcurrencySimulator.h
    src/concurrency/ConcurrencySimulator.cpp
)

target_link_libraries(CppMemoryAlgorithmVisualizer PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

target_include_directories(CppMemoryAlgorithmVisualizer PRIVATE src)

if(MSVC)
    target_compile_options(CppMemoryAlgorithmVisualizer PRIVATE /W4 /permissive-)
else()
    target_compile_options(CppMemoryAlgorithmVisualizer PRIVATE -Wall -Wextra -Wpedantic)
endif()
