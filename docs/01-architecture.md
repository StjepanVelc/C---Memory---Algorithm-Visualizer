# Arhitektura

## Cilj

Aplikacija vizualizira algoritme, memorijske koncepte i konkurentnost kroz unified simulation model.

## Visokorangirana Struktura

- `src/core`
  - Definira zajednicke tipove i lifecycle simulacije.
- `src/ui`
  - Render i kontrole korisnickog sucelja.
- `src/algorithms`
  - Generator frameova za search/sort scenarije.
- `src/memory`
  - Generator frameova za memory i data structure scenarije.
- `src/concurrency`
  - Generator frameova za thread/mutex scenarije.

## Klju cne Komponente

1. `SimulationTypes`
   - Canonical model frameova (`SimulationFrame`) koji svi generatori proizvode.
2. `SimulationEngine`
   - Upravljanje odabirom scenarija i step-forward/step-backward navigacijom.
3. `MainWindow`
   - Orkestrira tabove, kontrole, playback i export.
4. `VisualizationCanvas`
   - Renderira frameove u vizualni prikaz i animacije tranzicija.

## Dizajnerski Principi

- Jedan frame model za sve domene.
- Odvojena odgovornost generiranja podataka od UI renderiranja.
- Deterministicko step-by-step ponasanje za edukacijski kontekst.
- Niska coupling razina izmedu modula uz jednostavne API granice.
