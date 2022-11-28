#pragma once

#include "ponderer/dep/std/io/eof.hpp"

#include <istream>
#include <sstream>
#include <string>

namespace ponderer::std_ {

using ::std::basic_istream;
using ::std::string;

struct ReadResult {
	bool didSucceed = false;
	string content;
};

template<typename... Traits>
auto readUntil(basic_istream <Traits...>& is, decltype(eof)) -> ReadResult {
	::std::stringstream buffer;
	buffer << is.rdbuf();
	auto ret = ReadResult();
	ret.didSucceed = true;
	ret.content = buffer.str();
	return ret;
}

}
