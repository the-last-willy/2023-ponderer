#pragma once

#include "ponderer/core/windowing/GpuManager.hpp"

#include <ponderer/common/typing/all.hpp>
#include <ponderer/dep/imgui/all.hpp>

#include <functional>
#include <string>

namespace ponderer::windowing {

class Context;

enum class OpenFileStatus {
	undefined,

	found,
	notFound,

	count
};

struct OpenFileResult {
	OpenFileStatus status = {};
};

class MainInterface {

public:
	GpuManager gpuManager;

	bool hasFailedToFindFile = false;

	std::function<OpenFileResult(NullTerminated<const char*>)> onOpenFile
	= [](NullTerminated<const char*>) { return OpenFileResult(); };
};

void update(MainInterface&, Context&);

}

#include "ponderer/core/windowing/context.hpp"
