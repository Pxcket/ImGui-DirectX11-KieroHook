#include "Menu.h"
#include "../includes.h"
#include "../../include/imgui/imgui.h"

namespace UI {
    namespace Menu {
        void Render() {
            if (!globals::open) return;

            ImGui::Begin("Pxcket Menu", &globals::open, ImGuiWindowFlags_NoCollapse);

            if (ImGui::BeginTabBar("MainTabs")) {
                if (ImGui::BeginTabItem("Visuals")) {
                    
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Aim")) {

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Misc")) {

                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }
    }
}

