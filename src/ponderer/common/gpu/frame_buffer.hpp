#pragma once

#include "tags.hpp"

#include "ponderer/dep/glm/all.hpp"

#include <stdexcept>

namespace ponderer::gpu {

class FrameBuffer {
    GLuint server_ = GL_ZERO;

public:
    FrameBuffer() = default;

    FrameBuffer(GLuint server)
    : server_(server)
    {
        if(!glIsFramebuffer(this->server())) {
            throw std::logic_error("Bad resource.");
        }
    }

    FrameBuffer(decltype(to_acquire)) {
        glCreateFramebuffers(1, &server_);
    }

    ~FrameBuffer() {
        glDeleteFramebuffers(1, &server_);
    }

    auto has_resource() const -> bool {
        return true;
    }

    auto server() const -> GLuint {
        return server_;
    }

    operator GLuint() const {
        return server();
    }
};

}
