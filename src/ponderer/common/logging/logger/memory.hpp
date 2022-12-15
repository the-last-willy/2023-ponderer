#pragma once

#include "ponderer/common/logging/entry.hpp" // Entry

#include <vector>

namespace ponderer::log {

class MemoryLogger {
public:
	std::vector<Entry> entries_;

public:
	void log(const Entry& e) {
		entries_.push_back(e);
	}

	/// The memory logger can reuse the memory as-is without having to transform it.
	void log(Entry&& e) {
		entries_.push_back(std::move(e));
	}
};

}