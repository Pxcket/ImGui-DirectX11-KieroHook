#include "Menu.h"
#include "../includes.h"
#include "../../include/imgui/imgui.h"

namespace UI {
    namespace Menu {
        void Render() {
            if (!globals::open) return;

            ImGui::Begin("Pxcket Menu", &globals::open, ImGuiWindowFlags_NoCollapse);

            if (ImGui::BeginTabBar("MainTabs")) {
                if (ImGui::BeginTabItem("Tab 1")) {
                    
                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Tab 2")) {

                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Tab 3")) {

                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }
    }
}

