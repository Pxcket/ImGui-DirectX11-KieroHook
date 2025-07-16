#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "utils/libs/kiero/kiero.h"
#include "utils/libs/imgui/imgui.h"
#include "utils/libs/imgui/imgui_impl_win32.h"
#include "utils/libs/imgui/imgui_impl_dx11.h"


typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;



// Game Modules (Mostly Used By Il2cpp/Unity Games LMFAO)




uintptr_t ModuleGA = (uintptr_t)GetModuleHandleA("GameAssembly.dll"); // Game Assembly (Main func used by most il2cpp games)
uintptr_t ModuleUP = (uintptr_t)GetModuleHandleA("UnityPlayer.dll");
/*
uintptr_t ModuleMO = (uintptr_t)GetModuleHandleA("mono.dll");           
uintptr_t ModuleIL = (uintptr_t)GetModuleHandleA("il2cpp.dll");       // (Sometimes used instead of GameAssembly Just Check Ur Game First)
*/



// Unity Core & Backend



/*
uintptr_t ModuleCRT = (uintptr_t)GetModuleHandleA("UnityCRT.dll");        
uintptr_t ModuleAST = (uintptr_t)GetModuleHandleA("DataAccess.dll");   
uintptr_t ModuleINP = (uintptr_t)GetModuleHandleA("UnityInput.dll");   
*/



// Graphics & Physics



/*
uintptr_t ModulePHYSX = (uintptr_t)GetModuleHandleA("PhysX3_x64.dll");         
uintptr_t ModuleDX11 = (uintptr_t)GetModuleHandleA("d3d11.dll");             
uintptr_t ModuleVulkan = (uintptr_t)GetModuleHandleA("vulkan-1.dll");         
*/



// Anti-Cheat & DRM (Just Be Carefully Hooking These LMFAO)



/*
uintptr_t ModuleEAC = (uintptr_t)GetModuleHandleA("EasyAntiCheat.dll");   // Easy Anti-Cheat
uintptr_t ModuleBEC = (uintptr_t)GetModuleHandleA("BEClient.dll");        // BattlEye
uintptr_t ModuleDNO = (uintptr_t)GetModuleHandleA("Denuvo64.dll");        // Denuvo
*/