#pragma once

#include <ponderer/common/file/context.hpp>
#include <ponderer/dep/fmt/all.hpp>
#include <ponderer/dep/imgui/all.hpp>
#include <ponderer/dep/std/chrono/all.hpp>

namespace ponderer::windowing {

class FileInspector {
public:
	file::System* fileSystem_ = nullptr;
	file::FileId file_ = 0;

	bool isOpen_ = true;

public:

};

inline void update(FileInspector& inspector) {
	auto flags
		= ImGuiTableFlags_SizingFixedFit
		  | ImGuiTableFlags_RowBg
		  | ImGuiTableFlags_Borders
		  | ImGuiTableFlags_Resizable;
	auto name = fmt::format("File Inspector##{}", fmt::ptr(&inspector));
	if(auto window = ImGui_::Begin(name.c_str(), &inspector.isOpen_)) {
		auto& path = *inspector.fileSystem_->pathOf(inspector.file_);
		if(ImGui::CollapsingHeader("Info")) {
			if(auto table = ImGui_::BeginTable("Properties", 2, flags)) {
				ImGui::TableSetupColumn("Property");
				ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("ID");
					ImGui::TableNextColumn();
					ImGui::Text("%i", inspector.file_);
				}
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Path");
					ImGui::TableNextColumn();
					ImGui::Text("\"%s\"", path.string().c_str());
				}
			}
		}
		if(ImGui::CollapsingHeader("Cache")) {
			if(auto table = ImGui_::BeginTable("Properties", 2, flags)) {
				ImGui::TableSetupColumn("Property");
				ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::Text("Time stamp");
					ImGui::TableNextColumn();
					ImGui::Text("%i", inspector.file_);
				}
			}
			if(ImGui::Button("Load")) {

			}
			ImGui::SameLine();
			if(ImGui::Button("Unload")) {

			}
		}
		if(ImGui::CollapsingHeader("Disk")) {
			auto doesExist = std::filesystem::exists(path);
			if(!doesExist) {
				ImGui::Text("File does not exist on disk.");
			} else {
				if(auto table = ImGui_::BeginTable("Properties", 2, flags)) {
					ImGui::TableSetupColumn("Property");
					ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
					{
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text("Time stamp");
						ImGui::TableNextColumn();
						auto time = std::filesystem::last_write_time(path);
						auto timeStr = fmt::format("{:%Y/%m/%d-%H:%M:%S}", fmt::localtime(std_::to_time_t(time)));
						ImGui::Text("%s", timeStr.c_str());
					}
				}
			}
			ImGui::Text("");
			if(ImGui::Button("Save")) {

			}
		}
	}
}

}