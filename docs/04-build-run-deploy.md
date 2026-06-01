# Build, Run, Deploy

## Preduvjeti

- Visual Studio 2022 Community (Desktop development with C++).
- Qt 6.11.1 MSVC 2022 64-bit kit.
- CMake (koristi se i VS bundled CMake).

## Konfiguracija Okoline

```powershell
$env:CMAKE_PREFIX_PATH = "C:/Qt/6.11.1/msvc2022_64"
```

## Debug Build

```powershell
cmake --preset windows-debug
cmake --build --preset build-debug
```

## Pokretanje

```powershell
.\build\msvc-debug\Debug\CppMemoryAlgorithmVisualizer.exe
```

## Deploy Qt Runtime (po potrebi)

Ako ciljano pokretanje nema Qt DLL-ove, pokreni:

```powershell
windeployqt .\build\msvc-debug\Debug\CppMemoryAlgorithmVisualizer.exe
```

## Tipicni Problemi

1. `cmake` nije prepoznat
   - Koristi VS bundled cmake ili dodaj cmake u PATH.
2. `Qt6Config.cmake` nije pronaden
   - Provjeri `CMAKE_PREFIX_PATH` vrijednost.
3. x86/x64 mismatch
   - Koristi MSVC 2022 x64 preset i Qt `msvc2022_64` kit.
