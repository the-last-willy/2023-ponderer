#include "ponderer/file.hpp"
#include <ponderer/modeling.hpp>
#include <ponderer/shading.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_aligned.hpp>

#include <array>
#include <cassert>
#include <iostream>
#include <optional>
#include <stdexcept>

using namespace ponderer;

auto gl_debug_message_callback(
        GLenum source, GLenum type, GLuint id,
       GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    throw std::runtime_error(message);
}

auto read_model(const std::string& filepath)-> std::optional<Geometry> {
    auto importer = Assimp::Importer();
    auto scene = importer.ReadFile(filepath, 0);
    if(!scene) {
        throw std::runtime_error("Failed to read scene.");
    } else {
        auto indices = std::vector<int>();
        for(auto mi = 0; mi < scene->mNumMeshes; ++mi) {
            auto& mesh = *scene->mMeshes[mi];
            auto geom = Geometry();
            geom.vertex_count = mesh.mNumVertices;
            if(mesh.mPrimitiveTypes == aiPrimitiveType_POINT) {
                geom.mode = GL_POINTS;
            } else if(mesh.mPrimitiveTypes == aiPrimitiveType_LINE) {
                geom.mode = GL_LINES;
            } else if(mesh.mPrimitiveTypes == aiPrimitiveType_TRIANGLE) {
                geom.mode = GL_TRIANGLES;
            } else {
                throw std::runtime_error("Unsupported polygon format.");
            }
            {
                auto positions = std::vector<float>();
                positions.resize(3 * mesh.mNumVertices);
                for(auto vi = 0; vi < mesh.mNumVertices; ++vi) {
                    auto& vertex = mesh.mVertices[vi];
                    positions[3 * vi + 0] = vertex[0];
                    positions[3 * vi + 1] = vertex[2];
                    positions[3 * vi + 2] = -vertex[1];
                }
                glCreateBuffers(1, &geom.positions);
                glNamedBufferStorage(geom.positions, sizeof(float) * size(positions), data(positions), GL_NONE);
            }
            if(mesh.mColors != nullptr) {
                auto colors = std::vector<glm::packed_vec3>();
                colors.resize(mesh.mNumVertices);
                for(auto vi = 0; vi < mesh.mNumVertices; ++vi) {
                    auto& color = mesh.mColors[0][vi];
                    colors[vi] = glm::packed_vec3 (color[0], color[1], color[2]);
                }
                glCreateBuffers(1, &geom.colors);
                glNamedBufferStorage(geom.colors, sizeof(glm::packed_vec3) * size(colors), data(colors), GL_NONE);
            }
            return geom;
        }
    }
    return std::nullopt;
}

struct ProgramDescription {
    std::filesystem::path fragment_shader_path;
    std::filesystem::path vertex_shader_path;
};

GLint linked_program(const ProgramDescription& desc) {
    GLuint fragment_shader = GL_NONE;
    if(std::filesystem::exists(desc.fragment_shader_path)) {
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        auto content = ponderer::content(desc.fragment_shader_path);
        auto len = GLint(size(content));
        auto str = data(content);
        glShaderSource(fragment_shader, 1, &str, &len);
        glCompileShader(fragment_shader);
    } else {
        throw std::runtime_error("Failed to find fragment shader");
    }
    GLuint vertex_shader = GL_NONE;
    if(std::filesystem::exists(desc.vertex_shader_path)) {
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        auto content = ponderer::content(desc.vertex_shader_path);
        auto len = GLint(size(content));
        auto str = data(content);
        glShaderSource(vertex_shader, 1, &str, &len);
        glCompileShader(vertex_shader);
    } else {
        throw std::runtime_error("Failed to find vertex shader");
    }
    auto has_fragment_shader = fragment_shader != GL_NONE;
    auto has_vertex_shader = vertex_shader != GL_NONE;
    if(has_fragment_shader && has_vertex_shader) {
        auto program = glCreateProgram();
        glAttachShader(program, fragment_shader);
        glAttachShader(program, vertex_shader);
        glLinkProgram(program);
        return program;
    } else {
        throw std::runtime_error("Missing either fragment or vertex shader.");
    }
}

struct Application {
    GLFWwindow* window = nullptr;

    GLuint program = GL_NONE;

    std::optional<Geometry> geometry;

    GLuint vao = GL_NONE;

    modeling::Camera camera;

    Ssbo<Camera> gpu_camera;

    glm::vec2 last_mouse_pos = glm::vec2(0.f);
    glm::vec2 mouse_delta = glm::vec2(0.f);

    Application();
};

auto vao(const Geometry& g) -> GLuint {
    auto vao = GLuint(GL_NONE);
    glCreateVertexArrays(1, &vao);

    if(g.indices != GL_NONE) {
        glVertexArrayElementBuffer(vao, g.indices);
    }

    if(g.colors != GL_NONE) {
        glVertexArrayVertexBuffer(vao, 1, g.colors, 0, sizeof(glm::packed_vec3));
        glVertexArrayAttribBinding(vao, 1, 1);
        glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE,  0);
        glEnableVertexArrayAttrib(vao, 1);
    }

    if(g.positions != GL_NONE) {
        glVertexArrayVertexBuffer(vao, 0, g.positions, 0, sizeof(glm::packed_vec3));
        glVertexArrayAttribBinding(vao, 0, 0);
        glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE,  0);
        glEnableVertexArrayAttrib(vao, 0);
    }

    return vao;
}

Application::Application() {
    {
        auto model = read_model("D:\\data\\3d_scan\\thomas_flynn_raptor_model.ply");
        if(model) {
            geometry = *model;
        }
    }

    auto pdesc = ProgramDescription();
    pdesc.fragment_shader_path = "D:/dev/project/ponderer/src/ponderer/shading/basic.frag";
    pdesc.vertex_shader_path = "D:/dev/project/ponderer/src/ponderer/shading/basic.vert";
    program = linked_program(pdesc);

    if(geometry) {
        vao = ::vao(*geometry);
    }

    gpu_camera.client->world_to_view = glm::scale(glm::vec3(.5f));
}

void update(Application& a) {
    {
        auto x = 0.;
        auto y = 0.;
        glfwGetCursorPos(a.window, &x, &y);
        auto current_mouse_pos = glm::vec2(float(x), float(y));
        a.mouse_delta = current_mouse_pos - a.last_mouse_pos;
        a.last_mouse_pos = current_mouse_pos;
    }

    if(glfwGetMouseButton(a.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        a.camera.yaw -= a.mouse_delta.x / 2000.f;
        a.camera.pitch -= a.mouse_delta.y / 2000.f;
    }

    auto camera_rotation = rotation(a.camera);
    if(glfwGetKey(a.window, GLFW_KEY_A) == GLFW_PRESS)
    {
        auto dir = glm::vec4(modeling::left, 0.f) * camera_rotation;
        a.camera.position -= glm::vec3(dir.x, dir.y, dir.z) / 10.f;
    }
    if(glfwGetKey(a.window, GLFW_KEY_D) == GLFW_PRESS)
    {
        auto dir = glm::vec4(modeling::right, 0.f) * camera_rotation;
        a.camera.position -= glm::vec3(dir.x, dir.y, dir.z) / 10.f;
    }
    if(glfwGetKey(a.window, GLFW_KEY_S) == GLFW_PRESS)
    {
        auto dir = glm::vec4(modeling::forward, 0.f) * camera_rotation;
        a.camera.position -= glm::vec3(dir.x, dir.y, dir.z) / 10.f;
    }
    if(glfwGetKey(a.window, GLFW_KEY_W) == GLFW_PRESS)
    {
        auto dir = glm::vec4(modeling::forward, 0.f) * camera_rotation;
        a.camera.position += glm::vec3(dir.x, dir.y, dir.z) / 10.f;
    }

    a.gpu_camera.client->world_to_view = world_to_view(a.camera);
    a.gpu_camera.client->view_to_clip = view_to_clip(a.camera);

    glClearColor(37/255.f, 150/255.f, 190/255.f, 1.f);
    glClearDepth(100.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glUseProgram(a.program);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, a.gpu_camera.server);
    glBindVertexArray(a.vao);
    if(a.geometry) {
        if(a.geometry->indices) {
            glDrawElements(a.geometry->mode, a.geometry->vertex_count, GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(a.geometry->mode, 0, a.geometry->vertex_count);
        }
    }

}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void throwing_main() {
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    window = glfwCreateWindow(1600, 900, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwSwapInterval(1);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(&gl_debug_message_callback, nullptr);

    auto app = Application();
    app.window = window;

    while (!glfwWindowShouldClose(window))
    {
        update(app);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

auto main() -> int {
    try {
        throwing_main();
    } catch(const std::exception& e) {
        std::cerr << "std::exception: " << e.what() << std::endl;
    } catch(...) {
        std::cerr << "Unhandled exception." << std::endl;
    }
}
