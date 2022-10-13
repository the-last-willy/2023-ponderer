#pragma once

#include <ponderer/dependency/opengl.hpp>

namespace ponderer {

template<typename T>
struct Ssbo {
    GLuint server = GL_NONE;
    T* client = nullptr;

    Ssbo() {
        glCreateBuffers(1, &server);
        auto t = T();
        glNamedBufferStorage(server, sizeof(T), &t, GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT);
        client = static_cast<T*>(glMapNamedBufferRange(server, 0, sizeof(T), GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT));
    }

    ~Ssbo() {
        glUnmapNamedBuffer(server);
        glDeleteBuffers(1, &server);
    }
};

template<typename T>
void bind_to(const Ssbo<T>& ssbo, GLuint program, const char* name) {

}

}
