#pragma once

#include <ponderer/common/gpu/context/context.hpp>

namespace ponderer::windowing {

class GpuManager {
public:
	const gpu::Context* context = nullptr;

	bool isOpened = false;
};

void update(GpuManager&);

void updateObjects(GpuManager&);

void updateObjectsBuffers(GpuManager&);

}
