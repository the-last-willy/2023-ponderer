#pragma once

#include "ponderer/dependency/opengl.hpp"

namespace ponderer::modeling {

constexpr auto forward  = glm::vec3(0.f, 0.f, -1.f);
constexpr auto backward = glm::vec3(0.f, 0.f, +1.f);

constexpr auto up   = glm::vec3(0.f, -1.f, 0.f);
constexpr auto down = glm::vec3(0.f, +1.f, 0.f);

constexpr auto right    = glm::vec3(-1.f, 0.f, 0.f);
constexpr auto left     = glm::vec3(+1.f, 0.f, 0.f);

}
