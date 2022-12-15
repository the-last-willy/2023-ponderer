#pragma once

#include <imgui.h>

#include <utility>

namespace ponderer::ImGui_ {

struct BeginWrapper {
	bool b = false;

	BeginWrapper(bool b) : b(b) {}

	~BeginWrapper() {
		ImGui::End();
	}

	operator bool() const { return b; }
};

template<typename... Args>
[[nodiscard]] auto ScopedBegin(Args&&... args) -> BeginWrapper {
	auto b = ImGui::Begin(std::forward<Args>(args)...);
	return BeginWrapper(b);
}

struct ScopedBeginTable {
	bool b = false;

	ScopedBeginTable(bool b) : b(b) {}

	~ScopedBeginTable() {
		if(b) {
			ImGui::EndTable();
		}
	}

	operator bool() const { return b; }
};

template<typename... Args>
[[nodiscard]] auto BeginTable(Args&&... args) -> ScopedBeginTable {
	auto b = ImGui::BeginTable(std::forward<Args>(args)...);
	return ScopedBeginTable(b);
}

struct ScopedBeginMainMenuBar {
	bool b = false;

	ScopedBeginMainMenuBar(bool b) : b(b) {}

	~ScopedBeginMainMenuBar() {
		if(b) {
			ImGui::EndMainMenuBar();
		}
	}

	operator bool() const { return b; }
};

template<typename... Args>
[[nodiscard]] auto ScopedMainMenuBar(Args&&... args) -> ScopedBeginMainMenuBar {
	auto b = ImGui::BeginMainMenuBar(std::forward<Args>(args)...);
	return ScopedBeginMainMenuBar(b);
}

struct ScopedBeginMenu {
	bool b = false;

	ScopedBeginMenu(bool b) : b(b) {}

	~ScopedBeginMenu() {
		if(b) {
			ImGui::EndMenu();
		}
	}

	operator bool() const { return b; }
};

template<typename... Args>
[[nodiscard]] auto ScopedMenu(Args&&... args) -> ScopedBeginMenu {
	auto b = ImGui::BeginMenu(std::forward<Args>(args)...);
	return ScopedBeginMenu(b);
}

struct ScopedTreeNode {
	bool b = false;

	ScopedTreeNode(bool b) : b(b) {}

	~ScopedTreeNode() {
		if(b) {
			ImGui::TreePop();
		}
	}

	operator bool() const { return b; }
};

template<typename... Args>
[[nodiscard]] auto TreeNode(Args&&... args) -> ScopedTreeNode {
	auto b = ImGui::TreeNode(std::forward<Args>(args)...);
	return ScopedTreeNode(b);
}

}
