#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_aligned.hpp>

namespace ponderer {

struct Model {
    glm::aligned_mat4 global_to_view = glm::aligned_mat4(1.f);
    glm::aligned_mat4 view_to_projective = glm::aligned_mat4(1.f);
};

}
