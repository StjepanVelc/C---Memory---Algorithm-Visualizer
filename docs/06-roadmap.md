# Roadmap

## Status Implementacije

### Zavrseno

1. Mode-aware tooltips i mini tutorijal overlay.
2. Unit test target (`SimulatorTests`) + CTest integracija.
3. Open Exports Folder feedback u status baru.
4. Linked-list insert/delete at index.
5. Graph traversal modul (BFS).
6. Reproducibilni JSON presets.
7. Session recording + replay timeline.
8. Profiling overlay.
9. UI + frame text lokalizacija (hr/en).
10. MainWindow arhitekturno razdvajanje na manje implementacijske cjeline.

## Sljedece Profesionalne Iteracije

1. Dodati dedicated timeline scrubber (slider + event filtering po tipu).
2. Prosiriti i18n coverage na sve dinamicne row poruke kroz key-based generator API.
3. Uvesti CI pipeline za automatski build + test + artifact upload.
4. Dodati property-based testove za simulator edge-case kombinacije.

## Kriteriji Kvalitete

- Build reproducibilan kroz CMake presets.
- Bez runtime rušenja pri switching tabova i scenarija.
- Export JSON konzistentan i backwards-compatible.
