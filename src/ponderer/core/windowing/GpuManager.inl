#pragma once

#include "ponderer/core/windowing/GpuManager.hpp"

#include <ponderer/dep/fmt/all.hpp>
#include <ponderer/dep/imgui/all.hpp>

namespace ponderer::windowing {

inline void update(GpuManager& manager, Context& context) {
	if(manager.isOpened) {
		if(ImGui::Begin("GPU Manager", &manager.isOpened)) {
			if(manager.context != nullptr) {
				{
					auto majorVersion = 0;
					glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
					auto minorVersion = 0;
					glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
					ImGui::Text("Renderer  : %s", glGetString(GL_RENDERER));
					ImGui::Text("Vendor    : %s", glGetString(GL_VENDOR));
					ImGui::Text("Version   : %i.%i", majorVersion, minorVersion);
				}
				updateObjects(manager, context);
			} else {
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 0.f, 0.f, 1.f));
				ImGui::Text("No GPU context specified.");
				ImGui::PopStyleColor();
			}
		}
		ImGui::End();
	}
}

inline void updateObjects(GpuManager& manager, Context& context) {
	if(ImGui::CollapsingHeader("Objects")) {
		updateObjectsBuffers(manager);
		if(ImGui::TreeNode("Frame buffers")) {
			ImGui::TreePop();
		}
		updateObjectsPrograms(manager);
		if(ImGui::TreeNode("Vertex arrays")) {
			ImGui::TreePop();
		}
		updateObjectsTextures(manager, context);
		if(ImGui::TreeNode("Shaders")) {
			ImGui::TreePop();
		}
	}
}

inline void updateObjectsBuffers(GpuManager& manager) {
	if(ImGui::TreeNode("Buffers")) {
		ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
		auto selectedRow = -1;
		if(ImGui::BeginTable("table", 2, flags)) {
			ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableHeadersRow();
			for(auto&& [id, metadata] : manager.context->bufferMetadata_) {
				ImGui::TableNextRow();
				auto isSelected = false;
				ImGui::TableNextColumn();
				ImGui::Selectable(fmt::format("{}", id).c_str(), &isSelected, ImGuiSelectableFlags_SpanAllColumns);
				if(ImGui::IsItemClicked()) {
					selectedRow = ImGui::TableGetRowIndex();
				}
				ImGui::TableNextColumn();
				ImGui::Text("size");
			}
			ImGui::EndTable();

			ImGui::Text("Selected = %i", selectedRow);
		}
		ImGui::TreePop();
	}
}

inline void updateObjectsPrograms(GpuManager& manager) {
	if(ImGui::TreeNode("Programs")) {
		ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
		if(ImGui::BeginTable("table", 1, flags)) {
			ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableHeadersRow();
			for(auto&& [id, metadata] : manager.context->programToMetadata_) {
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%i", id);
			}
			ImGui::EndTable();
		}
		ImGui::TreePop();
	}
}

inline void updateObjectsTextures(GpuManager& manager, Context& context) {
	if(ImGui::TreeNode("Textures")) {
		ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
		auto selectedRow = -1;
		if(ImGui::BeginTable("table", 2, flags)) {
			ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableHeadersRow();
			for(auto&& [id, metadata] : manager.context->textureToMetadata_) {
				ImGui::TableNextRow();
				auto isSelected = false;
				ImGui::TableNextColumn();
				ImGui::Selectable(fmt::format("{}", id).c_str(), &isSelected, ImGuiSelectableFlags_SpanAllColumns);
				if(ImGui::IsItemClicked()) {
					openInspector(context, gpu::TextureId(id));
				}
				ImGui::TableNextColumn();
				ImGui::Text("type");
			}
			ImGui::EndTable();

			ImGui::Text("Selected = %i", selectedRow);
		}
		ImGui::TreePop();
	}
}

}
