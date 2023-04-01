#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

#include "Shader.hpp"
#include "Utils.hpp"
#include "Logging.hpp"

//---------------------------------

Shader::Shader():
	m_program_handle(0)
{}

Shader::~Shader()
{
	if (m_program_handle) glSafeCallVoid(glDeleteProgram(m_program_handle));
}

//---------------------------------

GLuint Shader::getNativeHandle()
{
	return m_program_handle;
}

//---------------------------------

bool Shader::loadFromMemory(const std::string& source, Shader::Type type)
{
	switch (type)
	{
		case Shader::Type::Vertex:
			return compile(source.data(), nullptr, nullptr);
			break;

		case Shader::Type::Fragment:
			return compile(nullptr, source.data(), nullptr);
			break;

		case Shader::Type::Geometry:
			return compile(nullptr, nullptr, source.data());
			break;

		default:
			break;
	}

	return false;
}

bool Shader::loadFromMemory(const std::string& vertex_source, const std::string& fragment_source, const std::string& geometry_source)
{
	return compile(vertex_source.data(), fragment_source.data(), geometry_source.data());
}

//---------------------------------

bool Shader::loadFromStream(std::ifstream& stream, Shader::Type type)
{
	std::vector <char> buffer;
	ReadStream(stream, &buffer);
	return loadFromMemory(buffer.data(), type);
}

bool Shader::loadFromStream(std::ifstream& vertex_stream, std::ifstream& fragment_stream, std::ifstream& geometry_stream)
{
	std::vector<char> vertex_buffer, fragment_buffer, geometry_buffer;
	ReadStream(vertex_stream,   &vertex_buffer  );
	ReadStream(fragment_stream, &fragment_buffer);
	ReadStream(geometry_stream, &geometry_buffer);
	return loadFromMemory(vertex_buffer.data(), fragment_buffer.data(), geometry_buffer.data());
}

//---------------------------------

bool Shader::loadFromFile(const std::filesystem::path& filename, Shader::Type type)
{
	std::ifstream stream;
	stream.open(filename, std::ios::binary);
	if (!stream)
	{
		LogError("Failed to open shader source '%s'\n", filename.string().c_str());
		return false;
	}

	bool result = loadFromStream(stream, type);

	stream.close();
	return result;
}

bool Shader::loadFromFile(const std::filesystem::path& vertex_filename, const std::filesystem::path& fragment_filename, const std::filesystem::path& geometry_filename)
{
	std::ifstream vertex_stream;
	vertex_stream.open(vertex_filename);
	if (!vertex_stream)
	{
		LogError("Failed to load vertex shader source '%s'\n", vertex_filename.string().c_str());
		return false;
	}

	std::ifstream fragment_stream;
	fragment_stream.open(fragment_filename);
	if (!fragment_stream)
	{
		vertex_stream.close();
		LogError("Failed to load fragment shader source '%s'\n", fragment_filename.string().c_str());
		return false;
	}

	std::ifstream geometry_stream;
	geometry_stream.open(geometry_filename);
	if (!geometry_stream)
	{
		vertex_stream.close();
		fragment_stream.close();
		LogError("Failed to load geometry shader source '%s'\n", geometry_filename.string().c_str());
		return false;
	}

	bool result = loadFromStream(vertex_stream, fragment_stream, geometry_stream);

	vertex_stream.close();
	fragment_stream.close();
	geometry_stream.close();
	return result;
}

//---------------------------------

bool Shader::compile(const char* vertex_source, const char* fragment_source, const char* geometry_source)
{
	// Deleting previously generated shader program
	if (m_program_handle) glSafeCallVoid(glDeleteProgram(m_program_handle));

	// Creating new programe
	m_program_handle = glSafeCall(glCreateProgram());

	static GLchar info[512] = "";
	static GLint  success   = 0;
	static GLint  loglen    = 0;

	if (vertex_source)
	{
		// Creating and compiling shader
		GLuint vertex_shader = glSafeCall(glCreateShader(GL_VERTEX_SHADER));
		glSafeCallVoid(glShaderSource(vertex_shader, 1, &vertex_source, nullptr));
		glSafeCallVoid(glCompileShader(vertex_shader));

		glSafeCallVoid(glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success));
		glSafeCallVoid(glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &loglen));
		if (loglen > 1)
		{
			glSafeCallVoid(glGetShaderInfoLog(vertex_shader, sizeof(info), NULL, info));

			if (!success)
			{
				LogError("Vertex shader compilation failed:\n%s\n", info);
				glSafeCallVoid(glDeleteShader(vertex_shader));
				return false;
			}

			LogWarning("Vertex shader compilation log:\n%s\n", info);
		}

		// Attaching to program and deleting shader, it's no mode needed
		glSafeCallVoid(glAttachShader(m_program_handle, vertex_shader));
		glSafeCallVoid(glDeleteShader(vertex_shader));
	}

	if (fragment_source)
	{
		// All same with the fragment shader
		GLuint fragment_shader = glSafeCall(glCreateShader(GL_FRAGMENT_SHADER));
		glSafeCallVoid(glShaderSource(fragment_shader, 1, &fragment_source, nullptr));
		glSafeCallVoid(glCompileShader(fragment_shader));

		glSafeCallVoid(glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success));
		glSafeCallVoid(glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &loglen));
		if (loglen > 1)
		{
			glSafeCallVoid(glGetShaderInfoLog(fragment_shader, sizeof(info), NULL, info));

			if (!success)
			{
				LogError("Fragment shader compilation failed:\n%s\n", info);
				glSafeCallVoid(glDeleteShader(fragment_shader));
				return false;
			}

			LogWarning("Fragment shader compilation log:\n%s\n", info);
		}

		glSafeCallVoid(glAttachShader(m_program_handle, fragment_shader));
		glSafeCallVoid(glDeleteShader(fragment_shader));
	}

	if (geometry_source)
	{
		// And geometry shader
		GLuint geometry_shader = glSafeCall(glCreateShader(GL_GEOMETRY_SHADER));
		glSafeCallVoid(glShaderSource(geometry_shader, 1, &geometry_source, nullptr));
		glSafeCallVoid(glCompileShader(geometry_shader));

		glSafeCallVoid(glGetShaderiv(geometry_shader, GL_COMPILE_STATUS, &success));
		glSafeCallVoid(glGetShaderiv(geometry_shader, GL_INFO_LOG_LENGTH, &loglen));
		if (loglen > 1)
		{
			glSafeCallVoid(glGetShaderInfoLog(geometry_shader, sizeof(info), NULL, info));

			if (!success)
			{
				LogError("Geometry shader compilation failed:\n%s\n", info);
				glSafeCallVoid(glDeleteShader(geometry_shader));
				return false;
			}

			LogWarning("Geometry shader compilation log:\n%s\n", info);
		}

		glSafeCallVoid(glAttachShader(m_program_handle, geometry_shader));
		glSafeCallVoid(glDeleteShader(geometry_shader));
	}

	// Linking program and checking result
	glSafeCallVoid(glLinkProgram(m_program_handle));
	glSafeCallVoid(glGetProgramiv(m_program_handle, GL_LINK_STATUS, &success));
	if (!success)
	{
		glSafeCallVoid(glGetProgramInfoLog(m_program_handle, sizeof(info), nullptr, info));
		LogError("Shader program linkage failed:\n%s\n", info);

		return false;
	}

	return true;
}

GLint Shader::getUniformLocation(const char* name)
{
	GLint location = glSafeCall(glGetUniformLocation(m_program_handle, name));
	if (location == -1)
		LogWarning("Uniform '%s' not found in shader\n", name);

	return location;
}

//---------------------------------

void Shader::use()
{
	glSafeCallVoid(glUseProgram(m_program_handle));
}

//---------------------------------

bool Shader::setUniform(const char* name, const glm::mat3& matrix)
{
	GLint location = getUniformLocation(name);
	if (location == -1)
		return false;

	glSafeCallVoid(glProgramUniformMatrix3fv(m_program_handle, location, 1, GL_FALSE, glm::value_ptr(matrix)));
	return true;
}

bool Shader::setUniform(const char* name, const glm::mat4& matrix)
{
	GLint location = getUniformLocation(name);
	if (location == -1)
		return false;

	glSafeCallVoid(glProgramUniformMatrix4fv(m_program_handle, location, 1, GL_FALSE, glm::value_ptr(matrix))); 
	return true;
}

bool Shader::setUniform(const char* name, const glm::vec3& vector)
{
	GLint location = getUniformLocation(name);
	if (location == -1)
		return false;

	glSafeCallVoid(glProgramUniform3f(m_program_handle, location, vector.x, vector.y, vector.z)); 
	return true;
}

bool Shader::setUniform(const char* name, const glm::vec4& vector)
{
	GLint location = getUniformLocation(name);
	if (location == -1)
		return false;

	glSafeCallVoid(glProgramUniform4f(m_program_handle, location, vector.x, vector.y, vector.z, vector.w)); 
	return true;
}

bool Shader::setUniform(const char* name, const Color& color)
{
	GLint location = getUniformLocation(name);
	if (location == -1)
		return false;

	glSafeCallVoid(glProgramUniform4f(m_program_handle, location, color.r, color.g, color.b, color.a)); 
	return true;
}

bool Shader::setUniform(const char* name, int value)
{
	GLint location = getUniformLocation(name);
	if (location == -1)
		return false;

	glSafeCallVoid(glProgramUniform1i(m_program_handle, location, value)); 
	return true;
}

bool Shader::setUniform(const char* name, float value)
{
	GLint location = getUniformLocation(name);
	if (location == -1)
		return false;

	glSafeCallVoid(glProgramUniform1f(m_program_handle, location, value)); 
	return true;
}

bool Shader::setUniform(const char* name, bool value)
{
	return setUniform(name, value? 1: 0);
}

bool Shader::setUniform(const char* name, const Texture& texture, int id)
{
	LogAssert(id >= 0 && id <= GL_TEXTURE31-GL_TEXTURE0 /* glActiveTexture accepts values from GL_TEXTURE0 to GL_TEXTURE31 */);
	if (!setUniform(name, id))
		return false;

	glSafeCallVoid(glActiveTexture(GL_TEXTURE0+id));
	texture.bind();
	return true;
}

//---------------------------------