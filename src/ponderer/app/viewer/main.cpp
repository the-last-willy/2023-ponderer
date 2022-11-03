#include "ponderer/common/file.hpp"
#include "ponderer/dep/glfw/all.hpp"
#include "ponderer/dep/imgui/all.hpp"
#include "ponderer/common/gpu.hpp"
#include "ponderer/common/modeling.hpp"
#include "ponderer/core/shading.hpp"
#include <ponderer/core/windowing/all.hpp>
#include <ponderer/dep/stb/all.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_aligned.hpp>

#include <array>
#include <cassert>
#include <filesystem>
#include <iostream>
#include <optional>
#include <stdexcept>

using namespace ponderer;

auto gl_debug_message_callback(
        GLenum source, GLenum type, GLuint id,
       GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    __debugbreak();
    throw std::runtime_error(message);
}

auto read_model(gpu::Context& gpu, const std::string& filepath)-> std::optional<Geometry> {
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

				geom.positions = acquireBuffer(gpu, std::span(positions), GL_NONE);
            }
            if(mesh.mColors != nullptr) {
                auto colors = std::vector<glm::packed_vec3>();
                colors.resize(mesh.mNumVertices);
                for(auto vi = 0; vi < mesh.mNumVertices; ++vi) {
                    auto& color = mesh.mColors[0][vi];
                    colors[vi] = glm::packed_vec3 (color[0], color[1], color[2]);
                }
				geom.colors = acquireBuffer(gpu, std::span(colors), GL_NONE);
            }
            return geom;
        }
    }
    return std::nullopt;
}

auto read_model_billboards(gpu::Context& c, const std::string& filepath)-> gpu::ShaderStorage<shading::Billboard[]> {
    auto importer = Assimp::Importer();
    auto scene = importer.ReadFile(filepath, 0);
    if(!scene) {
        throw std::runtime_error("Failed to read scene.");
    } else {
        auto indices = std::vector<int>();
        for(auto mi = 0; mi < scene->mNumMeshes; ++mi) {
            auto& mesh = *scene->mMeshes[mi];
            auto storage = gpu::ShaderStorage<shading::Billboard[]>(mesh.mNumVertices);
            {
                for(auto vi = 0; vi < mesh.mNumVertices; ++vi) {
                    auto& vertex = mesh.mVertices[vi];
                    storage.client()[vi].position = glm::packed_vec4(vertex[0], vertex[2], -vertex[1], 1.f);
                }
            }
            if(mesh.mColors != nullptr) {
                for(auto vi = 0; vi < mesh.mNumVertices; ++vi) {
                    auto& color = mesh.mColors[0][vi];
                    storage.client()[vi].color = glm::packed_vec4(color[0], color[1], color[2], color[3]);
                }
            }
            return storage;
        }
        throw std::runtime_error("No model");
    }
}

auto billboard_geometry(gpu::Context& gpu) -> Geometry {
    auto g = Geometry();
    g.mode = GL_TRIANGLES;
    { // Indices.
        auto data = std::vector<GLuint>{
            0, 2, 1,
            1, 2, 3,
        };
        g.vertex_count = size(data);
		g.indices = acquireBuffer(gpu, std::span(data));
    }
    { // Positions.
        auto data = std::vector<glm::packed_vec3>{
            glm::packed_vec3(-1.f, -1.f, 0.f),
            glm::packed_vec3(-1.f, +1.f, 0.f),
            glm::packed_vec3(+1.f, -1.f, 0.f),
            glm::packed_vec3(+1.f, +1.f, 0.f),
        };
		g.positions = acquireBuffer(gpu, std::span(data));
    }
    return g;
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

std::vector<shading::Billboard> billboard_grid() {
    auto ret = std::vector<shading::Billboard>();
    for(int i = 0; i < 5; ++i)
    for(int j = 0; j < 5; ++j) {
        auto&& b = ret.emplace_back();
        b.position = glm::packed_vec4(5.f * float(i), 5.f * float(j), 0.f, 1.f);
    }
    return ret;
}

struct Application {
	gpu::Context gpuContext;
    windowing::MainInterface mainInterface;

    int frame_count = 0;

    GLFWwindow* window = nullptr;

    GLuint program = GL_NONE;

    std::optional<Geometry> geometry;

    GLuint vao = GL_NONE;

    Geometry billboard_geom;
    GLuint billboard_program = GL_NONE;
    GLuint billboard_vao = GL_NONE;

    Ssbo<shading::Billboard> billboard_data;
    Ssbo<shading::Billboard[25]> billboard_instance_data;

    gpu::ShaderStorage<shading::Billboard[]> model_billboards;
    gpu::ShaderStorage<shading::DirectionalLight> gpu_directional_light;
    gpu::ShaderStorage<shading::PointLight> gpu_point_light;

    modeling::Camera camera;

    Ssbo<Camera> gpu_camera;

    glm::vec2 last_mouse_pos = glm::vec2(0.f);
    glm::vec2 mouse_delta = glm::vec2(0.f);

    Application();
};

auto vao(const Geometry& g) -> GLuint {
    auto vao = GLuint(GL_NONE);
    glCreateVertexArrays(1, &vao);

    if(g.indices) {
        glVertexArrayElementBuffer(vao, g.indices);
    }

    if(g.colors) {
        glVertexArrayVertexBuffer(vao, 1, g.colors, 0, sizeof(glm::packed_vec3));
        glVertexArrayAttribBinding(vao, 1, 1);
        glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE,  0);
        glEnableVertexArrayAttrib(vao, 1);
    }

    if(g.positions) {
        glVertexArrayVertexBuffer(vao, 0, g.positions, 0, sizeof(glm::packed_vec3));
        glVertexArrayAttribBinding(vao, 0, 0);
        glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE,  0);
        glEnableVertexArrayAttrib(vao, 0);
    }

    return vao;
}

Application::Application() {
    {
        // model_billboards = read_model_billboards("D:\\data\\3d_scan\\thomas_flynn_raptor_model.ply");
    }

//    auto pdesc = ProgramDescription();
//    pdesc.fragment_shader_path = "D:/dev/project/ponderer/src/ponderer/core/shading/basic.frag";
//    pdesc.vertex_shader_path = "D:/dev/project/ponderer/src/ponderer/core/shading/basic.vert";
//    program = linked_program(pdesc);

    if(geometry) {
        vao = ::vao(*geometry);
    }

    gpu_camera.client->world_to_view = glm::scale(glm::vec3(.5f));

    { // Billboard.
        auto pd = ProgramDescription();
        pd.fragment_shader_path = "D:\\dev\\project\\ponderer\\src\\ponderer/core\\shading\\billboard.frag";
        pd.vertex_shader_path = "D:\\dev\\project\\ponderer\\src\\ponderer/core\\shading\\billboard.vert";
        billboard_program = linked_program(pd);

        billboard_geom = billboard_geometry(gpuContext);
        billboard_vao = ::vao(billboard_geom);

        auto bg = billboard_grid();
        for(auto i = 0; i < size(bg); ++i) {
            (*billboard_instance_data.client)[i] = bg[i];
        }
    }

	{ // Interface.
		mainInterface.gpuManager.context = &gpuContext;
	}
}

void update(Application& a) {
    a.frame_count += 1;

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

    { // Directional light.
        auto world_dir = glm::vec4(1.f, 1.f, 1.f, 0.f);
        auto view_dir = a.gpu_camera.client->world_to_view * world_dir;
        a.gpu_directional_light.client().direction = glm::normalize(view_dir);
    }

    a.gpu_camera.client->world_to_view = world_to_view(a.camera);
    a.gpu_camera.client->view_to_clip = view_to_clip(a.camera);

    glClearColor(37/255.f, 150/255.f, 190/255.f, 1.f);
    glClearDepth(100.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

//    glUseProgram(a.billboard_program);
//    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, a.gpu_camera.server);
//    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, a.model_billboards.server());
//    // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, a.billboard_data.server);
//    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, a.gpu_directional_light.server());
//    glBindVertexArray(a.billboard_vao);
//
//    {
//        if(a.billboard_geom.indices) {
//            // glDrawElements(a.billboard_geom.mode, a.billboard_geom.vertex_count, GL_UNSIGNED_INT, 0);
//            glDrawElementsInstanced(a.billboard_geom.mode, a.billboard_geom.vertex_count, GL_UNSIGNED_INT, 0, a.model_billboards.client().size());
//        } else {
//            // glDrawArrays(a.billboard_geom.mode, 0, a.billboard_geom.vertex_co unt);
//            glDrawArraysInstanced(a.billboard_geom.mode, 0, a.model_billboards.client().size(), a.billboard_geom.vertex_count);
//        }
//    }

    if(glfwGetKey(a.window, GLFW_KEY_E) == GLFW_PRESS) {
        auto pixels = std::vector<std::byte>(1699 * 900 * 3);
        glReadPixels(0, 0, 1600, 900, GL_RGB, GL_UNSIGNED_BYTE, data(pixels));
        auto success = stbi_write_png("D:\\dev\\project\\ponderer\\tmp\\screenshot.png", 1600, 900, 3, data(pixels), 0);
        if(!success) {
            std::cerr << "Capture failed." << std::endl;
        }
    }

    if(glfwGetKey(a.window, GLFW_KEY_L) == GLFW_PRESS) {

    }

	update(a.mainInterface);
}

void throwing_main() {
    GLFWwindow* window;

    ponderer::glfwInit();

    window = glfwCreateWindow(1600, 900, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwSwapInterval(1);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(&gl_debug_message_callback, nullptr);

    auto app = Application();
    app.window = window;

    imGuiSetup(*window);

    while (!glfwWindowShouldClose(window))
    {
        imGuiNewFrame();

        update(app);

        ImGui::ShowDemoWindow();

        imGuiRender();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    imGuiCleanup();

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
