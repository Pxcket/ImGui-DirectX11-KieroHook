#pragma once
#include "../../ext/libs/imgui/imgui.h"

void CustomCursorDraw()
{
    ImGuiIO& io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetForegroundDrawList();

    ImVec2 mouse_pos = io.MousePos;
	ImU32 cursorcol = IM_COL32(255, 255, 255, 255); // RGBA conzept :D  (Red, Green, Blue, Alpha)

    float size = 8.0f;

    draw_list->AddLine(ImVec2(mouse_pos.x - size, mouse_pos.y), ImVec2(mouse_pos.x + size, mouse_pos.y), cursorcol, 2.0f);

    draw_list->AddLine(ImVec2(mouse_pos.x, mouse_pos.y - size), ImVec2(mouse_pos.x, mouse_pos.y + size), cursorcol, 2.0f);

	//This is a Simple + Cursor, you can change it to whatever you want
}
void MenuDraw()
{
	ImGui::Begin("Pxcket Unity", &vars->UI->ToggleMenu);
    {

    }
    ImGui::End();
}