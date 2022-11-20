#pragma once

#include <ponderer/dep/fmt/all.hpp>
#include <ponderer/common/gpu/all.hpp>
#include <ponderer/dep/imgui/all.hpp>

namespace ponderer::windowing {

///
class TextureInspector {
	bool isOpen_ = true;
	GLuint texture_ = GL_NONE;

	GLint selectedLevel_ = 0;

public:
	explicit TextureInspector(gpu::TextureId texture)
	: texture_(texture)
	{}

	void inspect(GLuint texture) {
		texture_ = texture;
	}

	auto isOpen() const -> bool {
		return isOpen_;
	}

	auto isOpen() -> bool& {
		return isOpen_;
	}

	auto selectedLevel() const -> GLint {
		return selectedLevel_;
	}

	auto setSelectedLevel(GLint i) {
		selectedLevel_ = i;
	}

	auto texture() const -> GLuint {
		return texture_;
	}
};

inline void updateMenuBar(TextureInspector& inspector) {
	// @BUG : Doesn't work for some reason, need to look into it.
	// if(ImGui::BeginMenuBar()) {
		if(ImGui::BeginMenu("Level")) {
			{
				auto levelCount = GetTextureParameter(inspector.texture(), gpu::gl::TEXTURE_BASE_LEVEL);
				for(auto i = 0; i <= levelCount; ++i) {
					auto isSelectedLevel = i == inspector.selectedLevel();
					auto isSelected = isSelectedLevel;
					ImGui::MenuItem(fmt::format("Level {}", i).c_str(), nullptr, &isSelected);
					if(isSelected && isSelected != isSelectedLevel) {
						inspector.setSelectedLevel(i);
					}
				}
			}
			ImGui::EndMenu();
		}
		ImGui::MenuItem("Metrics/Debugger");
		// ImGui::EndMenuBar();
	// }
}

inline void update(TextureInspector& inspector) {
	ImGui::SetNextWindowSizeConstraints(ImVec2(500, 500), ImVec2(FLT_MAX, FLT_MAX));
	if(ImGui::Begin(fmt::format("TextureInspector##{}", fmt::ptr(&inspector)).c_str(), &inspector.isOpen())) {
		updateMenuBar(inspector);

		{
			auto size = ImVec2(ImGui::GetContentRegionAvail().x * 0.75f, ImGui::GetContentRegionAvail().y);
			ImGui::BeginChild("Main pane", size);
			if(inspector.texture()) {
				auto id = std::size_t(inspector.texture());
				ImGui::Image(reinterpret_cast<ImTextureID>(id), ImVec2(500, 500));
			} else {
				ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(.5f, .5f));
				ImGui::Text("No texture selected.");
				ImGui::PopStyleVar();
			}
			ImGui::EndChild();
		}
		ImGui::SameLine();
		{
			auto size = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
			ImGui::BeginChild("Right panel", size);
			{
				auto target = GetTextureParameter(inspector.texture(), gpu::gl::TEXTURE_TARGET);
				ImGui::Text("Target=%s", gpu::gl::toString(target));
			}
			{
				auto levels = GetTextureParameter(inspector.texture(), gpu::gl::TEXTURE_BASE_LEVEL);
				ImGui::Text("Levels=%i", levels);
			}
			ImGui::Separator();
			{
				{
					auto width = GetTextureLevelParameter(inspector.texture(), 0, gpu::gl::TEXTURE_WIDTH);
					auto height = GetTextureLevelParameter(inspector.texture(), 0, gpu::gl::TEXTURE_HEIGHT);
					ImGui::Text("Resolution=%ix%i", width, height);
				}
			}

			ImGui::EndChild();
		}
	}
	ImGui::End();
}

}