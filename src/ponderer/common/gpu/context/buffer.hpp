#pragma once

#include "ponderer/common/gpu/context/context.hpp"
#include "ponderer/common/gpu/handles.hpp"
#include "ponderer/common/gpu/ids.hpp"

namespace ponderer::gpu {

template<typename... Ts>
auto acquireBuffer(Context& c, Ts&&... args) -> BufferHandle {
	auto&& b = c.buffers.emplace_back();
	{
		b = buffer2(std::forward<Ts>(args)...);
	}
	{
		auto&& info = c.bufferInfos.emplace_back();
		info.id = b;
	}
	return BufferHandle(c, BufferId(b));
}

inline void release(Context& c, BufferId id) {
	for(auto it = begin(c.bufferInfos); it != end(c.bufferInfos);) {
		if(it->id == id) {
			it = c.bufferInfos.erase(it);
		} else {
			++it;
		}
	}
}

}