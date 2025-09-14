#include "PresentHook.h"
#include "../UI/Overlay.h"
#include "../Core/HookManager.h"

static bool dxInitialized = false;

namespace Hooks {
    HRESULT __stdcall Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
        if (!dxInitialized) {
            if (InitializeDirectX(pSwapChain)) {
                dxInitialized = true;
                UI::Overlay::Initialize(window, pDevice, pContext);
                HookManager::InstallWndProcHook();
            }
            return oPresent(pSwapChain, SyncInterval, Flags);
        }

        UI::Overlay::Render();

        return oPresent(pSwapChain, SyncInterval, Flags);
    }

    bool InitializeDirectX(IDXGISwapChain* pSwapChain) {
        if (FAILED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice))) {
            return false;
        }

        pDevice->GetImmediateContext(&pContext);

        DXGI_SWAP_CHAIN_DESC sd;
        pSwapChain->GetDesc(&sd);
        window = sd.OutputWindow;

        ID3D11Texture2D* pBackBuffer;
        if (FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer))) {
            return false;
        }

        if (FAILED(pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView))) {
            pBackBuffer->Release();
            return false;
        }

        pBackBuffer->Release();
        return true;
    }
}