#pragma once

#include <ponderer/dep/std/container/map.hpp>

#include <filesystem>
#include <fstream>
#include <map>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <vector>

namespace ponderer::file {

using FileId = std::size_t;
using FilePath = std::filesystem::path;

struct FileCache {
	std::filesystem::file_time_type loadTimeStamp;
	std::filesystem::file_time_type fileTimeStamp;

	std::string content;
};

struct FileDisk {
	std::fstream stream;
};

struct File {
	std::optional<FileCache> cache;
	std::optional<FileDisk> disk;
	FilePath path;
};

class System {
	std::map<FileId, File> idToFile_;

	std::map<FilePath, FileId> pathToId_;

	FileId nextFreeId = 0;

	auto acquireId() -> FileId {
		return nextFreeId++;
	}

	auto dataPtrOf(FileId id) -> File* {
		return std_::findPtr(idToFile_, id);
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

	auto open(FileId id) {
		if(auto data = dataPtrOf(id)) {
			auto& disk = data->disk.emplace();
			disk.stream = std::fstream(data->path, std::ios::in | std::ios::out);
		} else {
			throw std::runtime_error("File ID not registered.");
		}
	}

	auto load(FileId id) {
		if(auto data = dataPtrOf(id)) {
			auto& cache = data->cache.emplace();

		} else {
			throw std::runtime_error("File ID not registered.");
		}
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