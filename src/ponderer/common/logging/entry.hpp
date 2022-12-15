#pragma once

#include "ponderer/common/logging/timestamp.hpp"

namespace ponderer::log {

class Entry {
public:
	Timestamp timestamp;

	std::string message;
};

}
