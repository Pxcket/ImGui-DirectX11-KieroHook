#include "WndProc.h"
#include "../UI/Overlay.h"
#include "../Core/HookManager.h"
#include "../includes.h"

namespace Hooks {
    LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (uMsg == WM_KEYUP) {
            if (wParam == VK_DELETE || wParam == VK_INSERT || wParam == VK_F9) {
                globals::open = !globals::open; //we gotta love the ghetto mouse fix method :)
            }
        }

        if (globals::open) {
            ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
            return true;
        }

        return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
    }

    bool InstallWndProcHook(HWND hWindow) {
        if (!hWindow || oWndProc) return false;

        oWndProc = (WNDPROC_t)SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);
        return oWndProc != nullptr;
    }

    void RemoveWndProcHook() {
        if (window && oWndProc) {
            SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)oWndProc);
            oWndProc = nullptr;
        }
    }
}