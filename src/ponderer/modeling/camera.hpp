#pragma once

#include <ponderer/dependency/opengl.hpp>

namespace ponderer::modeling {

struct Camera {
    float pitch = 0.f;
    float yaw = 0.f;

    float aspect_ratio = 16.f / 9.f;

    float near = 0.01f;
    float far = 100.f;

    float horizontal_fov = 90.f;

    glm::vec3 position = glm::vec3(0.f);
};

auto rotation(const Camera& c) -> glm::mat4 {
    auto pitch_axis = glm::vec3(1.f, 0.f, 0.f);
    auto pr = glm::rotate(-c.pitch, pitch_axis);
    auto yaw_axis = glm::vec3(0.f, 1.f, 0.f);
    auto yr = glm::rotate(-c.yaw, yaw_axis);
    return pr * yr;
}

auto world_to_view(const Camera& c) -> glm::mat4 {
    auto rot = rotation(c);
    auto tr = glm::translate(-c.position);
    return rot * tr;
}

auto view_to_clip(const Camera& c) -> glm::mat4 {
    auto vertical_fov = c.horizontal_fov / c.aspect_ratio;
    return glm::perspective(vertical_fov, c.aspect_ratio, c.near, c.far);
}

}