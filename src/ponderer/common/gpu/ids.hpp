#pragma once

#include <ponderer/dep/glad/all.hpp>

namespace ponderer::gpu {

template<typename Traits>
class ObjectId {
public:
	using Id = GLuint;

private:
	Id id_ = GL_NONE;

public:
	ObjectId() = default;

	explicit
	ObjectId(Id id) : id_(id) {}

	auto value() const -> Id { return id_; }

	operator Id() const {
		return value();
	}
};

struct BufferIdTraits {};
struct FramebufferIdTraits {};
struct ProgramIdTraits {};
struct ShaderIdTraits {};
struct TextureIdTraits {};
struct VertexArrayIdTraits {};

using BufferId = ObjectId<BufferIdTraits>;
using FramebufferId = ObjectId<FramebufferIdTraits>;
using ProgramId = ObjectId<ProgramIdTraits>;
using ShaderId = ObjectId<ShaderIdTraits>;
using TextureId = ObjectId<TextureIdTraits>;
using VertexArrayId = ObjectId<VertexArrayIdTraits>;

}
