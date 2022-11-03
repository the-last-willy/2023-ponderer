#pragma once

#include "resources.hpp"

#include "ponderer/dep/glm/all.hpp"

#include <span>

namespace ponderer::gpu {

template<typename...>
class ShaderStorage;

template<typename T>
class ShaderStorage<T> {
    Buffer<T> server_;
    T* client_ = nullptr;

public:
    ShaderStorage()
    : server_(GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT)
    {
        client_ = static_cast<T*>(glMapNamedBufferRange(server_, 0, sizeof(T),
            GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT));
    }

    auto client() const -> const T& {
        return *client_;
    }

    auto client() -> T& {
        return *client_;
    }

    auto server() const -> GLuint {
        return server_;
    }
};

template<typename T>
class ShaderStorage<T[]> {
    Buffer<T[]> server_;
    std::span<T> client_;

public:
    ShaderStorage() = default;

    explicit ShaderStorage(std::size_t size)
    : server_(size, GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT)
    {
        auto ptr = static_cast<T*>(glMapNamedBufferRange(server_, 0, size * sizeof(T),
            GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT));
        client_ = std::span(ptr, size);
    }

    auto client() const -> std::span<const T> {
        return client_;
    }

    auto client() -> std::span<T> {
        return client_;
    }

    auto server() const -> GLuint {
        return server_;
    }
};

template<typename T, typename U>
class ShaderStorage<T, U[]> {
    // TODO ?
};

}