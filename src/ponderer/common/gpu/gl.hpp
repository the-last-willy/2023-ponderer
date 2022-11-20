#pragma once

#include "ponderer/common/gpu/ids.hpp"

#include <span>

namespace ponderer::gpu::gl {

template<typename T>
void NamedBufferStorage(BufferId buffer, std::span<T> data, GLenum flags = GL_NONE) {
	glNamedBufferStorage(buffer, size(data), std::data(data), flags);
}

}
