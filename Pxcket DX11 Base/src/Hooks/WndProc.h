#pragma once
#include "../includes.h"

namespace Hooks {
    LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    bool InstallWndProcHook(HWND hWindow);
    void RemoveWndProcHook();
}