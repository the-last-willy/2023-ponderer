#pragma once

#include "ponderer/common/gpu/tags.hpp"

#include <ponderer/dep/glm/all.hpp>

#include <cstdlib>
#include <span>
#include <stdexcept>

namespace ponderer::gpu {

template<typename T = std::byte[]>
class Buffer {
    GLuint server_ = GL_NONE;

public:
    Buffer() = default;

    explicit Buffer(GLbitfield flags = GL_NONE) {
        glCreateBuffers(1, &server_);
        glNamedBufferStorage(server(), sizeof(T), nullptr, flags);
    }

    Buffer(const T& data, GLbitfield flags = GL_NONE) {
        glCreateBuffers(1, &server_);
        glNamedBufferStorage(server(), sizeof(T), &data, flags);
    }

    Buffer(const Buffer& o) = delete;

    Buffer(Buffer&& o)
    : server_(o.server_)
    {
        o.server_ = GL_NONE;
    }

    auto operator=(const Buffer&) -> Buffer& = delete;

    auto operator=(Buffer&& o) -> Buffer& {
        server_ = o.server_;
        o.server_ = GL_NONE;
    }

    ~Buffer() {
        glDeleteBuffers(1, &server_);
    }

    auto server() const -> GLuint {
        return server_;
    }

    operator GLuint() const {
        return server();
    }
};

template<typename T>
class Buffer<T[]> {
    GLuint server_ = GL_NONE;

public:
    Buffer() = default;

    explicit Buffer(std::size_t s, GLbitfield flags) {
        glCreateBuffers(1, &server_);
        glNamedBufferStorage(server(), s * sizeof(T), nullptr, flags);
    }

    Buffer(const Buffer& o) = delete;

    Buffer(Buffer&& o)
            : server_(o.server_)
    {
        o.server_ = GL_NONE;
    }

    auto operator=(const Buffer&) -> Buffer& = delete;

    auto operator=(Buffer&& o) -> Buffer& {
        server_ = o.server_;
        o.server_ = GL_NONE;
        return *this;
    }

    ~Buffer() {
        glDeleteBuffers(1, &server_);
    }

    auto server() const -> GLuint {
        return server_;
    }

    operator GLuint() const {
        return server();
    }
};

// TODO : Buffer<T, U[]> ?

template<typename Tr>
concept ObjectResourceTraits
= requires {
	typename Tr::Id;

	// TODO : Handle case when `acquire` requires parameters eg. `shader` and `texture`.
	{ Tr::acquire() } -> std::same_as<typename Tr::Id>;

	Tr::release(std::declval<typename Tr::Id>());
};

template<ObjectResourceTraits Tr>
class ObjectResource {
public:
	using Traits = Tr;
	using Id = typename Traits::Id;

private:
	Id id_ = Id(GL_NONE);

public:
	ObjectResource() = default;

	template<typename... Args>
	explicit ObjectResource(Args&&... args)
	: id_(Traits::acquire(std::forward<Args>(args)...))
	{}

	explicit ObjectResource(decltype(toAcquire))
	: id_(Traits::acquire())
	{}

	ObjectResource(const ObjectResource&) = delete;

	ObjectResource(ObjectResource&& other)
	: id_(other.drop())
	{}

	~ObjectResource() {
		Traits::release(id());
	}

	auto operator=(const ObjectResource&) -> ObjectResource& = delete;

	auto operator=(ObjectResource&& other) -> ObjectResource& {
		reset(other.drop());
		return *this;
	}

	auto id() const -> Id {
		return id_;
	}

	auto drop() -> Id {
		auto dropped = id();
		id_ = Id(GL_NONE);
		return dropped;
	}

	void release() {
		Traits::release(drop());
	}

	void reset(Id id) {
		Traits::release(this->id());
		id_ = id;
	}
};

struct BufferResourceTraits {
	using Id = BufferId;

	static auto acquire() -> Id {
		auto id = GLuint(GL_NONE);
		glCreateBuffers(1, &id);
		return BufferId(id);
	}

	static void release(BufferId buffer) {
		auto id = buffer.value();
		glDeleteBuffers(1, &id);
	}
};

struct FramebufferResourceTraits {
	using Id = GLuint;

	static auto acquire() -> Id {
		auto id = GLuint(GL_NONE);
		glCreateFramebuffers(1, &id);
		return id;
	}

	static void release(Id id) {
		glDeleteFramebuffers(1, &id);
	}
};

struct ProgramResourceTraits {
	using Id = GLuint;

	static auto acquire() -> Id {
		return glCreateProgram();
	}

	static void release(Id id) {
		glDeleteProgram(id);
	}
};

struct ShaderResourceTraits {
	using Id = GLuint;

	static auto acquire(GLenum type) -> Id {
		return glCreateShader(type);
	}

	static auto acquire() -> Id {
		throw std::logic_error(
			"The type of the shader is required in order to acquire the resource.");
	}

	static void release(Id id) {
		glDeleteShader(id);
	}
};

struct TextureResourceTraits {
	using Id = GLuint;

	static auto acquire(GLenum type) -> Id {
		auto id = GLuint(GL_NONE);
		glCreateTextures(type, 1, &id);
		return id;
	}

	static auto acquire() -> Id {
		throw std::logic_error(
			"The type of the texture is required in order to acquire the resource.");
	}

	static void release(Id id) {
		glDeleteTextures(1, &id);
	}
};

struct VertexArrayResourceTraits {
	using Id = GLuint;

	static auto acquire() -> Id {
		auto id = GLuint(GL_NONE);
		glCreateVertexArrays(1, &id);
		return id;
	}

	static void release(Id id) {
		glDeleteVertexArrays(1, &id);
	}
};

using BufferResource = ObjectResource<BufferResourceTraits>;
using FramebufferResource = ObjectResource<FramebufferResourceTraits>;
using ProgramResource = ObjectResource<ProgramResourceTraits>;
using ShaderResource = ObjectResource<ShaderResourceTraits>;
using TextureResource = ObjectResource<TextureResourceTraits>;
using VertexArrayResource = ObjectResource<VertexArrayResourceTraits>;

}
