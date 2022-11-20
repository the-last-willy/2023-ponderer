#pragma once

#include <ponderer/dep/glad/all.hpp>

#include <type_traits>

namespace ponderer::gpu::gl {

// Texture.

struct {
	using Type = GLint;

	operator GLenum() const {
		return GL_TEXTURE_BASE_LEVEL;
	}
} constexpr TEXTURE_BASE_LEVEL;

struct {
	using Type = GLint;

	operator GLenum() const {
		return GL_TEXTURE_TARGET;
	}
} constexpr TEXTURE_TARGET;

template<typename Parameter>
auto GetTextureParameter(GLuint texture, Parameter pname) -> typename Parameter::Type {
	auto params = typename Parameter::Type();
	if constexpr(std::is_same_v<typename Parameter::Type, GLint>) {
		glGetTextureParameteriv(texture, pname, &params);
		return params;
	} else {
		static_assert(std::is_same_v<typename Parameter::Type, GLint>);
	}
}

class TextureTarget {
private:
	GLenum value_ = GL_NONE;

public:
	constexpr TextureTarget() = default;

	constexpr TextureTarget(GLenum value)
	: value_(value)
	{}

	constexpr auto value() const -> GLenum {
		return value_;
	}

	constexpr operator GLenum() const {
		return value();
	}
};

constexpr inline auto toString(TextureTarget target) -> const char* {
	if(target.value() == GL_TEXTURE_2D) {
		return "GL_TEXTURE_2D";
	} else {
		return "";
	}
}

// Texture / level.

struct {
	using Type = GLint;

	operator GLenum() const {
		return GL_TEXTURE_HEIGHT;
	}
} TEXTURE_HEIGHT;

struct {
	using Type = GLint;

	operator GLenum() const {
		return GL_TEXTURE_WIDTH;
	}
} TEXTURE_WIDTH;

template<typename Parameter>
auto GetTextureLevelParameter(GLuint texture, GLint level, Parameter pname) -> typename Parameter::Type {
	auto params = typename Parameter::Type();
	if constexpr(std::is_same_v<typename Parameter::Type, GLint>) {
		glGetTextureLevelParameteriv(texture, level, pname, &params);
		return params;
	} else {
		static_assert(std::is_same_v<typename Parameter::Type, GLint>);
	}
}

}
