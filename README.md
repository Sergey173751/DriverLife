# ETS2 Driver Life 0.5 — starter project

This is a **buildable Windows x64 starter scaffold**, not yet a verified ETS2 plugin.

## What this project currently contains
- `DriverLife.dll`: native DLL scaffold with a shared-memory data structure.
- `DriverLifeOverlay.exe`: transparent, topmost Win32 overlay; F10 toggles visibility.
- GitHub Actions workflow that builds both binaries on `windows-latest`.
- A simple needs model and INI configuration.

## Important limitations
- The DLL is **not yet connected to SCS Telemetry SDK**. It initializes shared memory and publishes demonstration values only.
- The overlay therefore does not yet display live ETS2 telemetry or persist real driver needs.
- Do not copy this into the ETS2 plugin folder expecting full gameplay functionality.
- ETS2 1.62 compatibility has not been tested.

## Build
Push this repository to GitHub. Open **Actions** → **Build DriverLife (Windows x64)** → **Run workflow**.
When complete, download the `ETS2_Driver_Life_0.5-windows-x64` artifact.

## Next implementation step
Add the official SCS Telemetry SDK headers and implement the SDK callbacks in `src/DriverLife.cpp`. Then map real game time and telemetry events into the shared-memory state consumed by the overlay.
