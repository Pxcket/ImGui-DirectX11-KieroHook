#include "Base/Base.h"

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved) {
    switch (dwReason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hMod);
        CreateThread(nullptr, 0, Base::MainThread, hMod, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        Base::ShutdownThread();
        break;
    }
    return TRUE;
}