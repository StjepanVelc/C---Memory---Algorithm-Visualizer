# Data Flow

## 1. Odabir Scenarija

1. Korisnik odabire tab i scenarij u `MainWindow`.
2. `MainWindow::onScenarioChanged` poziva `SimulationEngine::setScenario`.
3. Engine trazi `FrameSequence` od odgovarajuceg simulator modula.

## 2. Navigacija i Playback

1. `Step Forward/Back` i `Play/Pause` mijenjaju aktivni frame index u engine-u.
2. `MainWindow::refreshView` cita `currentFrame()`.
3. `VisualizationCanvas::setFrame` prima frame i pokrece tranzicijske animacije.

## 2.1 Recording i Timeline Replay

1. `Record Session` poziva `SimulationEngine::beginRecording`.
2. Engine zapisuje evente (`frameIndex`, `action`, `timestampMs`) pri svakoj korisnickoj akciji.
3. `MainWindow::refreshTimeline` puni timeline listu.
4. Klik na timeline event radi `jumpToFrame(frameIndex)` i instant refresh prikaza.

## 3. Render Pipeline

`paintEvent` renderira domene po prisutnim podacima u frameu:

- `arrayValues` -> algoritamski blokovi.
- `stackCells`, `heapCells`, `pointerLinks` -> memory view.
- `stackValues`, `queueValues` -> DS view.
- `linkedListValues` -> linked-list view.
- `threadStates`, `mutexOwner` -> concurrency view.

## 4. Export Pipeline

1. `Export JSON + PNG` gumb hvata screenshot canvasa.
2. Frame metapodaci i vrijednosti serializiraju se u JSON.
3. Datoteke se spremaju u `exports/` direktorij s timestamp nazivom.

## 5. Lokalizacija Pipeline

1. UI labels se ucitavaju iz `resources/i18n/{lang}.json`.
2. Frame title/description/rows prolaze kroz `textMap` translaciju prije rendera.
3. Timeline akcije (`set-scenario`, `step-forward`, ...) prikazuju se lokalizirano.
