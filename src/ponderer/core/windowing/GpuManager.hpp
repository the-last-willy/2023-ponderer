#pragma once

#include <ponderer/common/gpu/context/context.hpp>

namespace ponderer::windowing {

class Context;

class GpuManager {
public:
	gpu::Context* context = nullptr;

	bool isOpened = false;
};

void update(GpuManager&, Context&);

void updateObjects(GpuManager&, Context&);

void updateObjectsBuffers(GpuManager&);
void updateObjectsPrograms(GpuManager&);
void updateObjectsTextures(GpuManager&, Context&);

}

#include "ponderer/core/windowing/context.hpp"
