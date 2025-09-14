#pragma once

namespace HookManager {
    bool Initialize();
    void Shutdown();

    bool InstallPresentHook();
    bool InstallWndProcHook();

    bool IsPresentHooked();
    bool IsWndProcHooked();
}