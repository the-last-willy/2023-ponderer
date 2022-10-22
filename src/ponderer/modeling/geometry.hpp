#pragma once

#include "ponderer/dependency/opengl.hpp"

#include <optional>

namespace ponderer {

class Geometry {
public:
    int vertex_count = 0;

    GLuint colors = GL_NONE;
    GLuint indices = GL_NONE;
    GLuint mode = GL_NONE;
    GLuint normals = GL_NONE;
    GLuint positions = GL_NONE;
};

}
