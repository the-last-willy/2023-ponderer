#pragma once

#include <ponderer/common/gpu/all.hpp>
#include <ponderer/dep/glm/all.hpp>

#include <optional>

namespace ponderer {

class Geometry {
public:
    int vertex_count = 0;

	gpu::BufferHandle colors;
	gpu::BufferHandle indices;
	gpu::BufferHandle normals;
	gpu::BufferHandle positions;

    GLuint mode = GL_NONE;
};

}
