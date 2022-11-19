#pragma once

#include "ponderer/common/gpu/handles.hpp"
#include "ponderer/common/gpu/resources.hpp"
#include "ponderer/common/gpu/tags.hpp"

#include <map>

namespace ponderer::gpu {

struct BufferMetadata {};
struct ProgramMetadata {};
struct TextureMetadata {};

class Context {
public:
	std::map<GLuint, BufferMetadata> bufferMetadata_;
	std::map<GLuint, ProgramMetadata> programToMetadata_;
	std::map<GLuint, TextureMetadata> textureToMetadata_;
};

auto acquireBuffer(Context& c) -> BufferHandle {
	auto buffer = BufferResource(toAcquire);
	{
		c.bufferMetadata_.emplace(buffer.id(), BufferMetadata());
	}
	return BufferHandle(c, std::move(buffer));
}

void releaseBuffer(Context& c, GLuint id) {
	c.bufferMetadata_.erase(id);
}

auto acquire(Context& c, decltype(programTag)) -> ProgramHandle {
	auto program = ProgramResource(toAcquire);
	{
		c.programToMetadata_.emplace(program.id(), ProgramMetadata());
	}
	return ProgramHandle(c, std::move(program));
}

void release(Context& c, ProgramId program) {
	c.programToMetadata_.erase(program);
}

auto acquire(Context& c, decltype(textureTag), GLenum target) -> TextureHandle {
	auto resource = TextureResource(target);
	{
		c.textureToMetadata_.emplace(resource.id(), TextureMetadata());
	}
	return TextureHandle(c, std::move(resource));
}

void release(Context& c, TextureId resource) {
	c.programToMetadata_.erase(resource);
}

}