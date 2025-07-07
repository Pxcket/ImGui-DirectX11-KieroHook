#pragma once
#include "includes.h"
#include "utils/globals.hpp"
#include "utils/c_logger.h"
#include "gui/int/assets/PoppinsSemiBold.h"
#include "gui/int/assets/gui styles.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;



LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    // there is a bug or glitch where this thread never ends Most Likely something in keirohook
    // when DEL keybind is active, it creates a ghost overlay and blocks user input
    // its annoying when unloaded, but not planning to fix it. if its even possible


	if (uMsg == WM_KEYUP && wParam == VK_DELETE)
    {
        vars->UI->ToggleMenu ^= 1;
		 
    }

    if (vars->UI->ToggleMenu)
    {
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
        return true;
    }

    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}




void CreateConsole()
{
    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
    SetConsoleTitleA("Pxcket Unity Console");
    system("cls");
    Log::log("Console Created & Allocated");

}


void InitGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);

	ImGuiStylePurple(); // press CNTRL + LMB to See Where its stored      or Just go to gui/int/assets/gui styles.h


    // Im Heavily In love with Poppins SemiBold
    ImFontConfig font;
    font.FontDataOwnedByAtlas = false;

    io.Fonts->AddFontFromMemoryTTF((void*)PoppinsSemiBold, sizeof(PoppinsSemiBold), 18.5f, &font);
}

