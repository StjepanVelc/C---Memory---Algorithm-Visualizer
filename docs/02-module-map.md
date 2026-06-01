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

## `src/ui`

- `MainWindow.h/.cpp`
  - Tabs, scenario selector, controls, autoplay, export.
- `VisualizationCanvas.h/.cpp`
  - Drawing pipeline za algorithms, memory, data structures i concurrency.
  - Animacije pointera i linked-list tranzicija.

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
