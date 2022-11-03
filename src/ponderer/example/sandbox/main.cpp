#include "ponderer/common/file.hpp"
#include "ponderer/dep/glad/all.hpp"
#include "ponderer/dep/glfw/all.hpp"
#include "ponderer/dep/glm/all.hpp"

#include <cmath>
#include <vector>

using namespace ponderer;

auto gl_debug_message_callback(
        GLenum source, GLenum type, GLuint id,
       GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    __debugbreak();
    throw std::runtime_error(message);
}

struct Particle {
	glm::uint idx;
	glm::uint value;
};

struct Application {
    Application();
};

void print(const std::vector<Particle>& particles) {
	std::cout << "Particles[" << size(particles) << "]" << std::endl;
	for(int i = 0; i < size(particles); ++i) {
		std::cout << "  [" << i << "] " << particles[i].idx << " " << particles[i].value << "\n";
	}
	std::cout << std::endl;
}

Application::Application() {
	auto shaderPath = std::filesystem::path("D:\\dev\\project\\ponderer\\src\\ponderer\\example\\sandbox\\particles.glsl");
	auto program = glCreateProgram();
	{
		GLuint shader = GL_NONE;
		if(std::filesystem::exists(shaderPath)) {
			shader = glCreateShader(GL_COMPUTE_SHADER);
			auto content = ponderer::content(shaderPath);
			auto len = GLint(size(content));
			auto str = data(content);
			glShaderSource(shader, 1, &str, &len);
			glCompileShader(shader);
		} else {
			throw std::runtime_error("Failed to find vertex shader");
		}
		glAttachShader(program, shader);
		glLinkProgram(program);
	}

	auto particles = std::vector<Particle>();
	for(auto i = std::size_t(0); i < 1'000'0000; ++i) {
		auto& p = particles.emplace_back();
		p.idx = i;
	}

	auto particleCounts = std::vector<int>(size(particles), 0);

	auto inputCountBuffer = GLuint(0);
	glCreateBuffers(1, &inputCountBuffer);
	{
		auto value = GLuint(size(particles));
		glNamedBufferStorage(inputCountBuffer, sizeof(value), &value, GL_DYNAMIC_STORAGE_BIT);
	}

	auto outputCountBuffer = GLuint(0);
	glCreateBuffers(1, &outputCountBuffer);
	{
		auto value = GLuint(0);
		glNamedBufferStorage(outputCountBuffer, sizeof(value), &value, GL_DYNAMIC_STORAGE_BIT);
	}

	auto particleBuffer = GLuint(0);
	glCreateBuffers(1, &particleBuffer);
	glNamedBufferStorage(particleBuffer, size(particles) * sizeof(Particle), data(particles), GL_DYNAMIC_STORAGE_BIT);

	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, inputCountBuffer, 0, 4);
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, outputCountBuffer, 0, 4);
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 2, particleBuffer, 0, size(particles) * sizeof(Particle));

	// print(particles);

	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	glUseProgram(program);
	auto groupSize = 256;
	glDispatchCompute((size(particles) + groupSize - 1) / groupSize, 1, 1);

	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	glm::uint newSize = 0;
	glGetNamedBufferSubData(outputCountBuffer, 0, sizeof(newSize), &newSize);
	particles.resize(newSize);
	glGetNamedBufferSubData(particleBuffer, 0, sizeof(Particle) * newSize, data(particles));

	for(auto&& p : particles) {
		particleCounts[p.idx] += 1;
	}

	for(auto i = 0; i < size(particleCounts); ++i) {
		auto pc = particleCounts[i];
		if(pc != 1) {
			std::cout << "Particle " << i << " found " << pc << " times.\n";
		}
	}

	// print(particles);
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

	glfwSwapBuffers(window);

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
