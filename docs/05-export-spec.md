# Export Specifikacija

## Namjena

Export mehanizam omogucava spremanje trenutnog stanja simulacije za analizu, dijeljenje i debug.

## Artefakti

Svaki export generira dvije datoteke:

1. PNG screenshot canvasa
2. JSON payload framea

Naziv datoteka:

`<scenario>_<yyyyMMdd_HHmmss>.png`

`<scenario>_<yyyyMMdd_HHmmss>.json`

Lokacija:

`exports/` u rootu projekta.

## JSON Polja

- `title`, `description`
- `step`, `totalSteps`
- `comparisons`, `swaps`
- `mode`, `scenario`
- `arrayValues`, `stackValues`, `queueValues`, `linkedListValues`
- `rows` (event log)
- `timestamp`
- `screenshot` (putanja do PNG datoteke)

## UI Integracija

- `Export JSON + PNG`
  - Pokrece snapshot i serializaciju.
- `Open Exports Folder`
  - Otvara export direktorij u file exploreru.
