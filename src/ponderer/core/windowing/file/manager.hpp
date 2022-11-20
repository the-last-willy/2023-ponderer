#pragma once

#include <ponderer/common/file/context.hpp>
#include <ponderer/core/windowing/inspecting/file.hpp>
#include <ponderer/dep/imgui/all.hpp>

#include <memory>
#include <vector>

namespace ponderer::windowing {

class FileManager {
public:
	file::System* fileSystem_ = nullptr;

	bool isOpen_ = true;

	std::vector<std::unique_ptr<FileInspector>> inspectors;

	FileManager() = default;
};

inline void updateFileTable(FileManager& manager) {
	auto flags
	= ImGuiTableFlags_SizingFixedFit
	| ImGuiTableFlags_RowBg
	| ImGuiTableFlags_Borders
	| ImGuiTableFlags_Resizable
	| ImGuiTableFlags_Reorderable
	| ImGuiTableFlags_Hideable;

	if(auto table = ImGui_::BeginTable("Files", 2, flags)) {
		ImGui::TableSetupColumn("Id");
		ImGui::TableSetupColumn("Path", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();
		for(auto id : manager.fileSystem_->allFileIds()) {
			ImGui::TableNextRow();
			auto isSelected = false;
			ImGui::TableNextColumn();
			auto pressed = ImGui::Selectable(fmt::format("{}", id).c_str(), &isSelected, ImGuiSelectableFlags_SpanAllColumns);
			ImGui::TableNextColumn();
			auto path = manager.fileSystem_->pathOf(id)->string();
			ImGui::Text(path.c_str());
			if(pressed) {
				auto&& inspector = manager.inspectors.emplace_back(new FileInspector());
				inspector->fileSystem_ = manager.fileSystem_;
				inspector->file_ = id;
			}
		}
	}
}

inline void update(FileManager& manager) {
	if(manager.isOpen_) {
		if(auto window = ImGui_::Begin("File Manager", &manager.isOpen_)) {
			updateFileTable(manager);
		}

		for(auto&& inspector : manager.inspectors) {
			windowing::update(*inspector);
		}
	}

}

}
