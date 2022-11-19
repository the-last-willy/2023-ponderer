#pragma once

#include "ponderer/common/gpu/ids.hpp"
#include "ponderer/common/gpu/resources.hpp"

namespace ponderer::gpu {

class Context;

template<typename Traits_>
class ObjectHandleWithContext {
public:
	using Traits = Traits_;

	using Resource = typename Traits::Resource;

private:
	Context* context_ = nullptr;
	Resource resource_;

public:
	ObjectHandleWithContext() = default;

	ObjectHandleWithContext(Context& c, Resource&& r)
	: context_(&c)
	, resource_(std::move(r))
	{}

	ObjectHandleWithContext(ObjectHandleWithContext&&) = default;

	~ObjectHandleWithContext() {
		if(context_ != nullptr) {
			releaseBuffer(context(), resource()->id());
		}
	}

	auto operator=(ObjectHandleWithContext&&) -> ObjectHandleWithContext& = default;

	auto context() const -> Context& {
		return *context_;
	}

	auto resource() const -> const Resource* {
		return &resource_;
	}

	auto resource() -> Resource* {
		return &resource_;
	}
};

/// The goal of the handle is to properly release the resource when it goes out of scope.
/// Therefore you can only have a single handle to a given object.
/// If the handle is to be shared, it needs to be wrapped in a share object eg. `std::shared_ptr`.
/// An `ObjectHandle` doesn't know how to acquire an object.
/// It only knows how to release the resource when the handle goes out of scope.
template<typename Traits>
class ObjectHandle : private ObjectHandleWithContext<Traits> {
public:
	using Resource = typename Traits::Resource;

	using ObjectHandleWithContext<Traits>::ObjectHandleWithContext;

	using ObjectHandleWithContext<Traits>::resource;

	auto operator*() const -> const Resource& {
		return resource();
	}

	operator bool() const {
		return resource() != GL_NONE;
	}
};

struct BufferHandleTraits {
	using Id = BufferId;
	using Resource = BufferResource;
};

struct FramebufferHandleTraits {
	using Id = FramebufferId;
	using Resource = FramebufferResource;
};

struct ProgramHandleTraits {
	using Id = ProgramId;
	using Resource = ProgramResource;
};

struct ShaderHandleTraits {
	using Id = ShaderId;
	using Resource = ShaderResource;
};

struct TextureHandleTraits {
	using Id = TextureId;
	using Resource = TextureResource;
};

struct VertexArrayHandleTraits {
	using Id = VertexArrayId;
	using Resource = VertexArrayResource;
};

using BufferHandle = ObjectHandle<BufferHandleTraits>;
using FramebufferHandle = ObjectHandle<FramebufferHandleTraits>;
using ProgramHandle = ObjectHandle<ProgramHandleTraits>;
using ShaderHandle = ObjectHandle<ShaderHandleTraits>;
using TextureHandle = ObjectHandle<TextureHandleTraits>;
using VertexArrayHandle = ObjectHandle<VertexArrayHandleTraits>;

inline auto bufferRawSize(GLuint id) -> GLint {
	auto byteSize_ = GLint(0);
	glGetNamedBufferParameteriv(id, GL_BUFFER_SIZE, &byteSize_);
	return byteSize_;
}

}
