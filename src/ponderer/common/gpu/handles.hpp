#pragma once

#include "ponderer/common/gpu/ids.hpp"
#include "ponderer/common/gpu/resources.hpp"

#include "ponderer/common/gpu/context/context.hpp"

namespace ponderer::gpu {

struct BufferHandleTraits {
	using Id = BufferId;
	using Resource = BufferResource;
};

template<typename Traits_>
class ObjectHandleWithContext {
public:
	using Traits = Traits_;

private:
	Context* context_ = nullptr;
	BufferResource resource_;

public:
	ObjectHandleWithContext() = default;

	ObjectHandleWithContext(Context& c, BufferResource&& r)
	: context_(&c)
	, resource_(std::move(r))
	{}

	ObjectHandleWithContext(ObjectHandleWithContext&&) = default;

	~ObjectHandleWithContext() {
		if(context_ != nullptr) {
			release(context(), BufferId(resource()));
		}
	}

	auto operator=(ObjectHandleWithContext&&) -> ObjectHandleWithContext& = default;

	auto context() const -> Context& {
		return *context_;
	}

	auto resource() const -> const BufferResource& {
		return resource_;
	}

	auto resource() -> BufferResource& {
		return resource_;
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

using BufferHandle = ObjectHandle<BufferHandleTraits>;

inline auto byteSize(const BufferResource& buffer) -> GLint {
	auto byteSize_ = GLint(0);
	glGetNamedBufferParameteriv(buffer, GL_BUFFER_SIZE, &byteSize_);
	return byteSize_;
}

}
