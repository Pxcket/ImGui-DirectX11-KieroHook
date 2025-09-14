#include "HookManager.h"
#include "../Hooks/PresentHook.h"
#include "../Hooks/WndProc.h"


Present_t oPresent = nullptr;
WNDPROC_t oWndProc = nullptr;
HWND window = nullptr;
ID3D11Device* pDevice = nullptr;
ID3D11DeviceContext* pContext = nullptr;
ID3D11RenderTargetView* mainRenderTargetView = nullptr;

namespace HookManager {
    bool Initialize() {
        if (kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success) {
            return false;
        }

        return InstallPresentHook();
    }

    void Shutdown() {
        kiero::shutdown();
    }

    bool InstallPresentHook() {
        return kiero::bind(8, (void**)&oPresent, Hooks::Present) == kiero::Status::Success;
    }

    bool InstallWndProcHook() {
        if (window && !oWndProc) {
            oWndProc = (WNDPROC_t)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)Hooks::WndProc);
            return oWndProc != nullptr;
        }
        return false;
    }

    bool IsPresentHooked() {
        return oPresent != nullptr;
    }

    bool IsWndProcHooked() {
        return oWndProc != nullptr;
    }
}