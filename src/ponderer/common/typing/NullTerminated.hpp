#pragma once

namespace ponderer {

template<typename T>
using NullTerminated = T;

using ZString = NullTerminated<const char*>;

}