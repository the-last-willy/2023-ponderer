#pragma once

#include <filesystem>
#include <fstream>
#include <map>
#include <optional>
#include <ranges>
#include <vector>

namespace ponderer::file {

using FileId = std::size_t;
using FilePath = std::filesystem::path;

struct FileCache {
	std::filesystem::file_time_type timeStamp;
	std::string content;
};

struct FileDisk {

};

struct File {
	std::optional<FileCache> cache;
	FilePath path;
};

class System {
	std::map<FileId, File> idToFile_;

	std::map<FilePath, FileId> pathToId_;

	FileId nextFreeId = 0;

	auto acquireId() -> FileId {
		return nextFreeId++;
	}
public:

	auto register_(const FilePath& path) -> FileId {
		auto id = acquireId();
		auto& file = idToFile_[id];
		file.path = path;
		pathToId_[file.path] = id;
		return id;
	}

	auto unregister(const FilePath& path) -> bool {
		auto id = idOf(path);
		if(!id) {
			return false;
		} else {
			idToFile_.erase(*id);
			pathToId_.erase(path);
			return true;
		}
	}

	auto load(FileId id) {

	}

	auto allFileIds() -> std::ranges::sized_range auto {
		auto firstOf = [](auto&& pair) -> auto& { return pair.first; };
		return idToFile_ | std::views::transform(firstOf);
	}

	auto idOf(const FilePath& path) const -> std::optional<FileId> {
		auto it = pathToId_.find(path);
		if(it == end(pathToId_)) {
			return std::nullopt;
		} else {
			return it->second;
		}
	}

	auto pathOf(FileId id) const -> const std::filesystem::path* {
		auto it = idToFile_.find(id);
		if(it == end(idToFile_)) {
			return nullptr;
		} else {
			auto& file = it->second;
			return &file.path;
		}
	}
};

}