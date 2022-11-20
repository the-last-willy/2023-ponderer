#pragma once

#include <imgui.h>

#include <utility>

namespace ponderer::ImGui_ {

struct ScopedBegin {
	bool b = false;

	ScopedBegin(bool b) : b(b) {}

	~ScopedBegin() {
		ImGui::End();
	}

	operator bool() const { return b; }
};

template<typename... Args>
[[nodiscard]] auto Begin(Args&&... args) -> ScopedBegin {
	auto b = ImGui::Begin(std::forward<Args>(args)...);
	return ScopedBegin(b);
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
[[nodiscard]] auto BeginMenu(Args&&... args) -> ScopedBeginMenu {
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
