#pragma once

#include "ponderer/core/windowing/GpuManager.hpp"
#include "ponderer/dep/imgui/all.hpp"

namespace ponderer::windowing {

inline void update(GpuManager& manager) {
	if(manager.isOpened) {
		if(ImGui::Begin("GPU Manager", &manager.isOpened)) {
			if(manager.context != nullptr) {
				updateObjects(manager);
			} else {
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.f, 0.f, 1.f));
				ImGui::Text("No GPU context specified.");
				ImGui::PopStyleColor();
			}
		}
		ImGui::End();
	}
}

inline void updateObjects(GpuManager& manager) {
	if(ImGui::CollapsingHeader("Objects")) {
		updateObjectsBuffers(manager);
		if(ImGui::TreeNode("Frame buffers")) {
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Programs")) {
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Vertex arrays")) {
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Textures")) {
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("Shaders")) {
			ImGui::TreePop();
		}
	}
}

inline void updateObjectsBuffers(GpuManager& manager) {
	if(ImGui::TreeNode("Buffers")) {
		ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
		if(ImGui::BeginTable("table", 1, flags)) {
			ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableHeadersRow();
			for(auto i = std::size_t(0); i < size(manager.context->buffers); ++i) {
				auto&& buffer = manager.context->buffers[i];
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%i", buffer.server());
				ImGui::TableSetColumnIndex(1);
				ImGui::Text("%i", byteSize(buffer));
			}
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

}
