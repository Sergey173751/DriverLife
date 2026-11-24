#include <windows.h>
#include <string>
#include <sstream>
#include "DriverLifeShared.h"

static HWND g_window = nullptr;
static bool g_visible = false;
static HANDLE g_mapping = nullptr;
static DriverLifeSharedState* g_state = nullptr;

static std::wstring PercentLine(const wchar_t* label, float value) {
    std::wstringstream s;
    s << label << L": " << static_cast<int>(value + 0.5f) << L"%";
    return s.str();
}

static void DrawTextLine(HDC dc, int y, const std::wstring& text) {
    TextOutW(dc, 24, y, text.c_str(), static_cast<int>(text.size()));
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_PAINT) {
        PAINTSTRUCT ps{};
        HDC dc = BeginPaint(hwnd, &ps);
        SetBkMode(dc, TRANSPARENT);
        SetTextColor(dc, RGB(245, 245, 245));
        HFONT font = CreateFontW(22, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                                 DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                 CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");
        HGDIOBJ old = SelectObject(dc, font);
        DrawTextLine(dc, 18, L"DRIVER LIFE 0.5");
        float h=100, t=100, f=0, hy=100, to=100;
        if (g_state && g_state->magic == kDriverLifeMagic) {
            h=g_state->hunger; t=g_state->thirst; f=g_state->fatigue;
            hy=g_state->hygiene; to=g_state->toilet;
        }
        DrawTextLine(dc, 62, PercentLine(L"Голод", h));
        DrawTextLine(dc, 96, PercentLine(L"Жажда", t));
        DrawTextLine(dc, 130, PercentLine(L"Усталость", f));
        DrawTextLine(dc, 164, PercentLine(L"Гигиена", hy));
        DrawTextLine(dc, 198, PercentLine(L"Туалет", to));
        DrawTextLine(dc, 246, L"F10 — скрыть / показать");
        SelectObject(dc, old);
        DeleteObject(font);
        EndPaint(hwnd, &ps);
        return 0;
    }
    if (msg == WM_HOTKEY && wp == 1) {
        g_visible = !g_visible;
        ShowWindow(hwnd, g_visible ? SW_SHOWNOACTIVATE : SW_HIDE);
        return 0;
    }
    if (msg == WM_DESTROY) {
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int) {
    g_mapping = OpenFileMappingW(FILE_MAP_READ, FALSE, kSharedMemoryName);
    if (g_mapping) g_state = static_cast<DriverLifeSharedState*>(
        MapViewOfFile(g_mapping, FILE_MAP_READ, 0, 0, sizeof(DriverLifeSharedState)));

    const wchar_t* cls = L"DriverLifeOverlayWindow";
    WNDCLASSW wc{};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = instance;
    wc.lpszClassName = cls;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    RegisterClassW(&wc);

    g_window = CreateWindowExW(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        cls, L"DriverLife", WS_POPUP, 40, 40, 330, 310,
        nullptr, nullptr, instance, nullptr);
    if (!g_window) return 1;

    SetLayeredWindowAttributes(g_window, RGB(0, 0, 0), 220, LWA_ALPHA);
    RegisterHotKey(g_window, 1, 0, VK_F10);
    ShowWindow(g_window, SW_HIDE);

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
        InvalidateRect(g_window, nullptr, FALSE);
    }

    UnregisterHotKey(g_window, 1);
    if (g_state) UnmapViewOfFile(g_state);
    if (g_mapping) CloseHandle(g_mapping);
    return 0;
}
