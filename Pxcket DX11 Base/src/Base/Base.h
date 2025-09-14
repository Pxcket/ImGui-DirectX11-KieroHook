#pragma once
#include <Windows.h>
namespace Base {
    DWORD WINAPI MainThread(LPVOID lpReserved);
    void ShutdownThread();

    extern bool IsInitialized;
    extern bool IsRunning;
}