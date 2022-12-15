#pragma once

#include <ponderer/common/communicating/all.hpp>

#include <ponderer/dep/fmt/all.hpp>
#include <ponderer/dep/glfw/all.hpp>
#include <ponderer/dep/imgui/all.hpp>

#include <chrono>
#include <functional>
#include <stdexcept>
#include <vector>

using namespace ponderer;



class Log {
public:
	std::vector<LogEntry> entries_;

	void add(LogEntry entry) {
		entry.timestamp.time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		entries_.push_back(std::move(entry));
	}

	void clear() {
		entries_.clear();
	}

	auto entryRef(std::size_t i) const -> const LogEntry& {
		return entries_.at(i);
	}

	auto entryCount() const -> std::size_t {
		return size(entries_);
	}
};

class LogViewer {
	std::time_t lastClearTime = 0;

public:
	Log* log_ = nullptr;

	std::function<void()> onClear = [](){};

	auto logRef() const -> const Log& {
		if(!log_) {
			throw std::runtime_error("");
		} else {
			return *log_;
		}
	}

	void update() {
		if(!log_) return;
		if(auto window = ImGui_::ScopedBegin("Log")) {
			static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
			ImVec2 outer_size = ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 8);
			if(auto table = ImGui_::BeginTable("Logs", 2, flags, outer_size)) {

				ImGui::TableSetupScrollFreeze(0, 1);
				ImGui::TableSetupColumn("Time");
				ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableHeadersRow();

				ImGuiListClipper clipper;
				clipper.Begin(logRef().entryCount());
				while (clipper.Step())
				{
					for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
					{
						auto& entry = logRef().entryRef(row);

						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text(fmt::format("{:%H:%M:%S}", fmt::localtime(entry.timestamp.time)).c_str());
						ImGui::TableNextColumn();
						ImGui::Text(entry.message.c_str());
					}
				}
			}
			if(ImGui::Button("Clear")) {
				lastClearTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			}
		}
	}
};

class LogTestWindow {
public:
	com::StatefulCallback<void(const LogEntry&)> onLog;

	void update() {
		if(auto window = ImGui_::ScopedBegin("Log Tester")) {
			static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
			static char text[1024] = "";
			ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
			if(ImGui::Button("Send")) {
				auto entry = LogEntry();
				entry.message = text;
				onLog(entry);
			}
		}
	}
};
