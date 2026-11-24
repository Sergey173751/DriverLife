#include <windows.h>
#include "DriverLifeShared.h"

static HANDLE g_mapping = nullptr;
static DriverLifeSharedState* g_state = nullptr;

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);
        g_mapping = CreateFileMappingW(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE,
                                       0, sizeof(DriverLifeSharedState), kSharedMemoryName);
        if (g_mapping) {
            g_state = static_cast<DriverLifeSharedState*>(
                MapViewOfFile(g_mapping, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(DriverLifeSharedState)));
            if (g_state) {
                g_state->magic = kDriverLifeMagic;
                g_state->version = 5;
                g_state->hunger = 100.0f;
                g_state->thirst = 100.0f;
                g_state->fatigue = 0.0f;
                g_state->hygiene = 100.0f;
                g_state->toilet = 100.0f;
                g_state->enabled = TRUE;
            }
        }
    } else if (reason == DLL_PROCESS_DETACH) {
        if (g_state) UnmapViewOfFile(g_state);
        if (g_mapping) CloseHandle(g_mapping);
    }
    return TRUE;
}

// NOTE: This is only a DLL/bootstrap scaffold. SCS Telemetry SDK callbacks
// and game-time integration must be implemented before this is an ETS2 plugin.
