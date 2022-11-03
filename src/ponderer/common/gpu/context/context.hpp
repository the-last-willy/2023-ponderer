#pragma once

#include <vector>

namespace ponderer::gpu {

struct BufferInfo {
	GLuint id = GL_NONE;
};

class Context {

public:
	std::vector<BufferInfo> bufferInfos;

	// Resources.

	std::vector<BufferResource> buffers;
};



}