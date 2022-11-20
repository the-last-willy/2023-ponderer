#pragma once

#include <ponderer/common/file/context.hpp>
#include <ponderer/dep/imgui/all.hpp>

#include <array>

namespace ponderer::windowing {

class OpenFileDialog {
public:
	file::System* fileSystem_ = nullptr;

	bool isOpen_ = true;
	std::string errorMessage;

	OpenFileDialog() = default;

	auto context() const -> file::System& {
		return *fileSystem_;
	}
};

inline void update(OpenFileDialog& dialog) {
	if(dialog.isOpen_) {
		if(auto window = ImGui_::Begin("Open File", &dialog.isOpen_)) {
			static auto buffer = std::array<char, 256>();
			ImGui::InputText("Path", data(buffer), size(buffer));
			if(!dialog.errorMessage.empty()) {
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.f, 0.f, 1.0f));
				ImGui::Text(dialog.errorMessage.c_str());
				ImGui::PopStyleColor();
			}
			if(ImGui::Button("Open")) {
				auto path = std::filesystem::path(data(buffer));
				if(std::filesystem::exists(path)) {
					dialog.context().register_(path);
				} else {
					dialog.errorMessage = "File not found.";
				}
			}
			if(ImGui::Button("Open test file")) {
				auto path = std::filesystem::path("D:\\data\\quaternius\\Ultimate Stylized Nature - May 2022-20221113T092120Z-001\\Ultimate Stylized Nature - May 2022\\OBJ\\BirchTree_1.mtl");
				if(std::filesystem::exists(path)) {
					dialog.context().register_(path);
				} else {
					dialog.errorMessage = "File not found.";
				}
			}
		}
	}

}

}
