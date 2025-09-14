#include "Overlay.h"
#include "Menu.h"
#include "../includes.h"
#include "../../include/imgui/imgui.h"
#include "../../include/imgui/imgui_impl_win32.h"
#include "../../include/imgui/imgui_impl_dx11.h"

namespace UI {
    namespace Overlay {
        static bool s_Initialized = false;

        bool Initialize(HWND hWindow, ID3D11Device* device, ID3D11DeviceContext* context) {
            if (s_Initialized) return true;

            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

            ImGui_ImplWin32_Init(hWindow);
            ImGui_ImplDX11_Init(device, context);

            s_Initialized = true;
            return true;
        }

        void Shutdown() {
            if (!s_Initialized) return;

            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();

            s_Initialized = false;
        }

        void Render() {
            if (!s_Initialized) return;
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            if (globals::open)
            {
                Menu::Render(); // you can do "if (!globals::open) return;" on top of "Menu::Render" void but i dont care
            }

            ImGui::Render();
            pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }

        bool HandleInput(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
            return ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
        }
    }
}