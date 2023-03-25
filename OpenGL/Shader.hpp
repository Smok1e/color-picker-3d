#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <cstdarg>

#include "Color.hpp"
#include "Texture.hpp"
#include "Utils.hpp"

//---------------------------------

// Class that represents and manipulates shader program
class Shader
{
public:
	enum Type
	{
		Vertex,
		Fragment
	};

	class UniformBinder 
	{
	public:
		constexpr UniformBinder(Shader* shader, const char* name):
			m_shader(shader),
			m_name(name)
		{}

		template<typename T>
		bool operator=(T value);

	protected:
		Shader* m_shader;
		const char* m_name;

	};

	Shader();
	Shader(const Shader& copy) = delete; // Copying not allowed, sosi hui

	~Shader();

	// Returns native OpenGL shader handle
	GLuint getNativeHandle();

	// Loads and compiles shader program with specified type from source code
	bool loadFromMemory(const std::string& source, Shader::Type type);

	// Loads and compiles shader program with both types of shaders from source code
	bool loadFromMemory(const std::string& vertex_source, const std::string& fragment_source);

	// Loads and compiles shader program with specified type from stream
	bool loadFromStream(std::ifstream& stream, Shader::Type type);

	// Loads and compiles shader program with both types of shaders from stream
	bool loadFromStream(std::ifstream& vertex_stream, std::ifstream& fragment_stream);

	// Loads and compiles shader program with specified type from file
	bool loadFromFile(const std::filesystem::path& filename, Shader::Type type);

	// Loads and compiles shader program with both types of shaders from file
	bool loadFromFile(const std::filesystem::path& vertex_filename, const std::filesystem::path& fragment_filename);

	// Binds the program
	void use();

	Shader& operator=(const Shader& that) = delete; // Copying not allowed, di nah cerf

	// Uniforms
	bool setUniform(const char* name, const glm::mat4& matrix);
	bool setUniform(const char* name, const glm::vec3& vector);
	bool setUniform(const char* name, const glm::vec4& vector);
	bool setUniform(const char* name, const Color& color);
	bool setUniform(const char* name, int   value);
	bool setUniform(const char* name, float value);
	bool setUniform(const char* name, bool  value);
	bool setUniform(const char* name, const Texture& texture);

	// Uniform binder that allows expressions such as shader_instance["uniform_name"] = uniform_value;
	[[nodiscard]]
	constexpr UniformBinder operator[](const char* name) {
		return UniformBinder(this, name);
	}

	// Allows to format uniform name with printf style
	template<typename T>
	bool setUniformFormatted(T value, const char* format, ...);

	// Allows to set uniform array element by element index and array name
	template<typename T>
	bool setUniformArrayElement(const char* name, size_t index, T value);

protected:
	GLuint m_program_handle;

	bool compile(const char* vertex_source, const char* fragment_source);
	GLint getUniformLocation(const char* name);

};

//---------------------------------

template<typename T>
bool Shader::setUniformFormatted(T value, const char* format, ...)
{
	static char name[BUFFSIZE] = "";
	va_list args = {};
	va_start(args, format);
	vsprintf_s(name, format, args);
	va_end(args);

	return setUniform(name, value);
}

template<typename T>
bool Shader::setUniformArrayElement(const char* name, size_t index, T value)
{								   
	return setUniformFormatted(value, "%s[%zu]", name, index);
}

//---------------------------------

template<typename T>
bool Shader::UniformBinder::operator=(T value)
{
	return m_shader->setUniform(m_name, value);
}

//---------------------------------