#pragma once
#include <windows.h>

constexpr wchar_t kSharedMemoryName[] = L"Local\\ETS2_DriverLife_05";

struct DriverLifeSharedState {
    DWORD magic;
    DWORD version;
    float hunger;
    float thirst;
    float fatigue;
    float hygiene;
    float toilet;
    BOOL enabled;
};

constexpr DWORD kDriverLifeMagic = 0x44524C35; // "DRL5"
