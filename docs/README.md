# Dokumentacija Projekta

Ovaj direktorij sadrzi tehnicku dokumentaciju za C++ Memory & Algorithm Visualizer.

## Sadrzaj

1. `docs/01-architecture.md`
   - Arhitektura aplikacije, slojevi i odgovornosti.
2. `docs/02-module-map.md`
   - Pregled svih modula (`core`, `ui`, `memory`, `algorithms`, `concurrency`) i njihovih API granica.
3. `docs/03-data-flow.md`
   - Kako podaci putuju od scenarija do renderiranja.
4. `docs/04-build-run-deploy.md`
   - Build, run i deploy workflow za Windows + Qt.
5. `docs/05-export-spec.md`
   - Specifikacija export mehanizma (JSON + PNG).
6. `docs/06-roadmap.md`
   - Daljnji razvojni plan i prioriteti.

## Brzi Start

1. Konfiguriraj Qt putanju:

```powershell
$env:CMAKE_PREFIX_PATH = "C:/Qt/6.11.1/msvc2022_64"
```

2. Konfiguriraj i buildaj:

```powershell
cmake --preset windows-debug
cmake --build --preset build-debug
```

3. Pokreni aplikaciju:

```powershell
.\build\msvc-debug\Debug\CppMemoryAlgorithmVisualizer.exe
```
