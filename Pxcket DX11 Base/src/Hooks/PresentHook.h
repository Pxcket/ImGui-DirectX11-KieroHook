#pragma once
#include "../includes.h"

namespace Hooks {
    HRESULT __stdcall Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

    bool InitializeDirectX(IDXGISwapChain* pSwapChain);
    void CleanupDirectX(); // not really used anymore prob gonna remove it or re-do it
}