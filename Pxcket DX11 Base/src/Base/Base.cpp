#include "Base.h"
#include "../Core/Core.h"

namespace Base {
    bool IsInitialized = false;
    bool IsRunning = true;

    DWORD WINAPI MainThread(LPVOID lpReserved) {
        while (!Core::Initialize() && IsRunning) {
            Sleep(100);
        }

        if (IsRunning) {
            IsInitialized = true;
        }

        return 0;
    }

    void ShutdownThread() {
        IsRunning = false;
        Core::Shutdown();
    }
}