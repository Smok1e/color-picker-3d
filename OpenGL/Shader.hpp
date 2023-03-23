#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include "Color.hpp"

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

	Shader& operator= (const Shader& that) = delete; // Copying not allowed, di nah cerf

	// Uniforms
	bool setUniform(const char* name, const glm::mat4& matrix);
	bool setUniform(const char* name, const glm::vec3& vector);
	bool setUniform(const char* name, const glm::vec4& vector);
	bool setUniform(const char* name, const Color& color);
	bool setUniform(const char* name, int   value);
	bool setUniform(const char* name, float value);

protected:
	GLuint m_program_handle;

	bool compile(const char* vertex_source, const char* fragment_source);
	GLint getUniformLocation(const char* name);

};

//---------------------------------