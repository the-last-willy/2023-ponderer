#pragma once

#include "ponderer/dep/glm/all.hpp"

namespace ponderer::shading {

struct DirectionalLight {
    glm::packed_vec4 direction;
};

}
