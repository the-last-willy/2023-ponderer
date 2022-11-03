#pragma once

#include "ponderer/dep/glm/all.hpp"

#include <cstdlib>
#include <span>

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

class BufferResource {
	GLuint server_ = GL_NONE;

public:
	BufferResource() = default;

	template<typename T>
	BufferResource(const T& data, GLbitfield flags = GL_NONE)
	: BufferResource(sizeof(data), data, flags)
	{}

	template<typename T>
	BufferResource(GLsizeiptr size, GLbitfield flags = GL_NONE) {
		glCreateBuffers(1, &server_);
		glNamedBufferStorage(server(), size, nullptr, flags);
	}

	template<typename T>
	BufferResource(GLsizeiptr size, const T& data, GLbitfield flags = GL_NONE) {
		glCreateBuffers(1, &server_);
		glNamedBufferStorage(server(), size, &data, flags);
	}

	BufferResource(const BufferResource& o) = delete;

	BufferResource(BufferResource&& o)
	: server_(o.server_)
	{
		o.server_ = GL_NONE;
	}

	auto operator=(const BufferResource&) -> BufferResource& = delete;

	auto operator=(BufferResource&& o) -> BufferResource& {
		server_ = o.server_;
		o.server_ = GL_NONE;
		return *this;
	}

	~BufferResource() {
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
auto buffer2(std::span<T> data, GLbitfield flags = GL_NONE) -> BufferResource {
	return BufferResource(data.size_bytes(), std::data(data), flags);
}

template<typename... Args>
auto buffer2(Args&&... args) -> BufferResource {
	return BufferResource(std::forward<Args>(args)...);
}

}
