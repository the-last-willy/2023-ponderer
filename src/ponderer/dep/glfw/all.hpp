#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace ponderer {

inline void glfwErrorCallback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

inline void glfwInit() {
    glfwSetErrorCallback(&glfwErrorCallback);

    if(!::glfwInit()) {
        std::exit(EXIT_FAILURE);
    } else {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE , GLFW_OPENGL_CORE_PROFILE);
    }
}

}
