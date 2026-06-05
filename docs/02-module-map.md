# Modul Mapa

## `src/core`

- `SimulationTypes.h`
  - Enum scenarija i strukture podataka za render.
- `SimulationEngine.h/.cpp`
  - API:
    - `setScenario(Scenario)`
    - `stepForward()`
    - `stepBackward()`
    - `reset()`
    - `currentFrame()`
- `PresetIO.h/.cpp`
  - Stabilan JSON preset format i parsiranje (round-trip testirano).

## `src/ui`

- `MainWindow.h`
  - Deklaracija UI kontrola i slotova.
- `MainWindow.cpp`
  - Core konstruktor + playback slotovi.
- `MainWindowUi.cpp`
  - UI composition, refresh, mode tooltips, tutorial overlay, timeline list.
- `MainWindowActions.cpp`
  - Input apply, preset save/load, recording/replay, export, language handling.
- `MainWindowSupport.h/.cpp`
  - Shared helper funkcije (scenario lists, i18n load, path resolve).
- `VisualizationCanvas.h/.cpp`
  - Drawing pipeline za algorithms, memory, data structures i concurrency.
  - Animacije pointera i linked-list tranzicija.
  - Profiling overlay i graph traversal prikaz.

## `src/algorithms`

- `AlgorithmSimulator.h/.cpp`
  - Search i sort frame sequence generator.

## `src/memory`

- `MemorySimulator.h/.cpp`
  - Memory lifecycle scenariji (`new/delete`, `unique_ptr`, copy/move).
  - Data structure scenariji (`stack`, `queue`, `linked list`).

## `src/concurrency`

- `ConcurrencySimulator.h/.cpp`
  - Thread/mutex i race-condition scenariji.

## Granice Ovisnosti

- `ui` ovisi o `core` + simulatorima kroz `SimulationEngine`.
- `core` ovisi o simulator modulima, ali simulatori medusobno ne ovise.
- `simulator` moduli ovise samo o `SimulationTypes` modelu.
