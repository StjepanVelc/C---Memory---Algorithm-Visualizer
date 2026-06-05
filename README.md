# C++ Memory & Algorithm Visualizer

A desktop educational visualizer built with Qt 6 and modern C++. It explains how classic algorithms, memory models, data structures, and concurrency primitives behave through deterministic, step-by-step simulation frames.

## Highlights

- Unified simulation engine for all domains (algorithms, memory, data structures, concurrency)
- Interactive controls: step forward/backward, autoplay, speed control, reset
- Custom scenario input for values and operation parameters
- JSON preset save/load for reproducible runs
- Session recording with replay timeline (jump to recorded frame)
- Graph traversal visualization (BFS)
- Export snapshot as JSON + PNG
- UI and frame content localization (Croatian/English)
- Basic simulator test suite integrated with CTest

## Tech Stack

- C++20
- Qt 6 (Core, Gui, Widgets)
- CMake + CMake Presets
- MSVC 2022 toolchain (Windows)

## Project Structure

- src/core: shared types, simulation engine, preset serialization
- src/ui: MainWindow orchestration and VisualizationCanvas rendering
- src/algorithms: algorithm frame generators
- src/memory: memory and data-structure frame generators
- src/concurrency: thread/mutex/race-condition generators
- tests: simulator and preset compatibility tests
- docs: architecture, module map, data flow, build/deploy, export spec, roadmap

## Build (Windows, Debug)

```powershell
$env:CMAKE_PREFIX_PATH = "C:/Qt/6.11.1/msvc2022_64"
cmake --preset windows-debug
cmake --build --preset build-debug
```

## Run

```powershell
.\build\msvc-debug\Debug\CppMemoryAlgorithmVisualizer.exe
```

## Run Tests

```powershell
cmake --build --preset build-debug --target SimulatorTests
"C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/ctest.exe" --test-dir build/msvc-debug --output-on-failure -C Debug
```

## Documentation

See docs/README.md for the complete documentation index (HR + EN).

## License

This project currently has no explicit license file in the repository.
