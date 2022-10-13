#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_aligned.hpp>

namespace ponderer {

struct Camera {
    glm::aligned_mat4 world_to_view = glm::aligned_mat4(1.f);
    glm::aligned_mat4 view_to_clip = glm::aligned_mat4(1.f);
};

}
