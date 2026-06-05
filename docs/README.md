# Documentation Index / Indeks Dokumentacije

## EN

This folder contains the technical documentation for C++ Memory & Algorithm Visualizer.

### Contents

1. docs/01-architecture.md
   Architecture, layers, responsibilities, and MainWindow split strategy.
2. docs/02-module-map.md
   Module map for core, ui, algorithms, memory, and concurrency.
3. docs/03-data-flow.md
   End-to-end flow: scenario selection, frame generation, rendering, recording, replay timeline.
4. docs/04-build-run-deploy.md
   Build/run/deploy workflow for Windows + Qt (MSVC kit).
5. docs/05-export-spec.md
   JSON + PNG export format and behavior.
6. docs/06-roadmap.md
   Implementation status and future roadmap.

### Quick Start

```powershell
$env:CMAKE_PREFIX_PATH = "C:/Qt/6.11.1/msvc2022_64"
cmake --preset windows-debug
cmake --build --preset build-debug
.\build\msvc-debug\Debug\CppMemoryAlgorithmVisualizer.exe
```

## HR

Ovaj direktorij sadrzi tehnicku dokumentaciju za C++ Memory & Algorithm Visualizer.

### Sadrzaj

1. docs/01-architecture.md
   Arhitektura, slojevi odgovornosti i strategija razdvajanja MainWindow komponente.
2. docs/02-module-map.md
   Mapa modula core, ui, algorithms, memory i concurrency.
3. docs/03-data-flow.md
   Tok podataka od odabira scenarija do rendera, snimanja i replay timeline prikaza.
4. docs/04-build-run-deploy.md
   Build/run/deploy workflow za Windows + Qt (MSVC kit).
5. docs/05-export-spec.md
   Specifikacija JSON + PNG izvoza.
6. docs/06-roadmap.md
   Status implementacije i daljnji plan razvoja.
