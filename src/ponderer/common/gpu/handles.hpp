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

	~ObjectHandleWithContext() {
		if(context_ != nullptr) {
			release(context(), resource());
		}
	}

	auto context() const -> Context& {
		return *context_;
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
	using ObjectHandleWithContext<Traits>::ObjectHandleWithContext;

	using ObjectHandleWithContext<Traits>::id;
	using ObjectHandleWithContext<Traits>::operator bool;
	using ObjectHandleWithContext<Traits>::operator typename Traits::Id;
};

using BufferHandle = ObjectHandle<BufferHandleTraits>;

inline auto byteSize(const BufferHandle& buffer) -> GLint {
	auto byteSize_ = GLint(0);
	glGetNamedBufferParameteriv(buffer, GL_BUFFER_SIZE, &byteSize_);
	return byteSize_;
}

}
