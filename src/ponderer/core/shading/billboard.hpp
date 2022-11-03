#pragma once



namespace ponderer::shading {

struct Billboard {
    glm::aligned_vec4 horizontal_axis = glm::aligned_vec4(1.f, 0.f, 0.f, 0.f);
    glm::aligned_vec4 vertical_axis = glm::aligned_vec4(0.f, 1.f, 0.f, 0.f);

    glm::aligned_vec4 position = glm::aligned_vec4(0.f, 0.f, 0.f, 1.f);
    glm::aligned_vec4 color;

    glm::float32 scale = 1.f;
};

}