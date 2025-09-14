#pragma once
#include <d3d11.h>

namespace UI {
    namespace Overlay {
        bool Initialize(HWND hWindow, ID3D11Device* device, ID3D11DeviceContext* context);
        void Shutdown();

        void Render();

        bool HandleInput(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    }
}