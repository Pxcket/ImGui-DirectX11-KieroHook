#pragma once


#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>


#include "../include/kiero/kiero.h"


#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_dx11.h"
#include "../include/imgui/imgui_impl_win32.h"


typedef HRESULT(__stdcall* Present_t)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC_t)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;


namespace Hooks {
    HRESULT __stdcall Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
    LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}


extern Present_t oPresent;
extern WNDPROC_t oWndProc;
extern HWND window;
extern ID3D11Device* pDevice;
extern ID3D11DeviceContext* pContext;
extern ID3D11RenderTargetView* mainRenderTargetView;


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace globals {
    inline bool open = false;
}