#pragma once

#include <ctime> // std::time_t

namespace ponderer::log {

class Timestamp {
public:
	std::time_t time_ = 0;
};

}
