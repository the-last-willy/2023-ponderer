#pragma once

#include <ponderer/dep/glad/all.hpp>

namespace ponderer::gpu {

struct BufferIdTraits {};

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

using BufferId = ObjectId<BufferIdTraits>;

}
