#include "MainInterface.hpp"

#include "ponderer/dep/imgui/all.hpp"

namespace ponderer::windowing {

void update(MainInterface& interface, Context& context) {
    auto isOpenFileModal = false;
    if(ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("Open")) {
                isOpenFileModal = true;
            }
            ImGui::EndMenu();
        }
		if (ImGui::BeginMenu("Resource"))
		{
			if(ImGui::MenuItem("File")) {

			}
			if(ImGui::MenuItem("GPU")) {
				interface.gpuManager.isOpened = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tool"))
		{
			if(ImGui::MenuItem("Profiler")) {

			}
			ImGui::EndMenu();
		}
        ImGui::EndMainMenuBar();
    }
    if(isOpenFileModal) {
        ImGui::OpenPopup("Open file");
    }
    if (ImGui::BeginPopupModal("Open file", nullptr))
    {
        static char buf1[64] = "";
        ImGui::InputText("Path",     buf1, 64);
		if(interface.hasFailedToFindFile) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.f, 0.f, 1.f));
			ImGui::Text("Could not find `%s`.", buf1);
			ImGui::PopStyleColor();
		}
        if(ImGui::Button("Open")) {
			auto&& res = interface.onOpenFile(buf1);
			if(res.status == OpenFileStatus::found) {
				interface.hasFailedToFindFile = false;
				ImGui::CloseCurrentPopup();
			} else {
				interface.hasFailedToFindFile = true;
			}
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel")) {
			interface.hasFailedToFindFile = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

	update(interface.gpuManager, context);
}

}