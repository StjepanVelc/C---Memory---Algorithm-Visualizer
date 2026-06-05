# Architecture / Arhitektura

## EN

### Goal

The application visualizes algorithms, memory concepts, data structures, and concurrency using a unified frame model.

### High-Level Structure

- src/core
  Defines shared simulation data contracts and engine behavior.
- src/ui
  Contains rendering, controls, input, localization, and timeline interactions.
- src/algorithms
  Generates algorithm-oriented frame sequences.
- src/memory
  Generates memory/data-structure frame sequences.
- src/concurrency
  Generates thread/mutex/race-condition frame sequences.

### MainWindow Refactoring

MainWindow implementation was split to reduce file size and improve maintainability:

- src/ui/MainWindow.cpp
  Constructor and core playback/navigation slots.
- src/ui/MainWindowUi.cpp
  UI composition, scenario loading, view refresh, tooltips/tutorial, timeline interaction.
- src/ui/MainWindowActions.cpp
  Input parsing, preset I/O, recording/replay, export/open-folder, localization logic.
- src/ui/MainWindowSupport.cpp
  Shared helper utilities for scenario lists, project root resolution, i18n loading.

### Key Architectural Additions

- Full frame-text localization pipeline through text-map translation.
- Replay timeline panel with click-to-jump frame navigation.
- Preset serialization abstraction (src/core/PresetIO.cpp) for testable compatibility.

## HR

### Cilj

Aplikacija vizualizira algoritme, memorijske koncepte, strukture podataka i konkurentnost kroz jedinstveni frame model.

### Nova organizacija MainWindow sloja

- src/ui/MainWindow.cpp
  Konstruktor i osnovni playback/navigacijski slotovi.
- src/ui/MainWindowUi.cpp
  Slaganje UI-a, ucitavanje scenarija, refresh prikaza, tooltip/tutorijal, timeline interakcija.
- src/ui/MainWindowActions.cpp
  Parsiranje inputa, preset I/O, recording/replay, export/open-folder, lokalizacija.
- src/ui/MainWindowSupport.cpp
  Zajednicki helperi za scenarije, root putanje i i18n ucitavanje.
