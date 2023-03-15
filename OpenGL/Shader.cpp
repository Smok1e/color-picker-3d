#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

#include "Shader.hpp"
#include "Utils.hpp"

//---------------------------------

Shader::Shader():
	m_program_handle(0)
{}

Shader::~Shader()
{
	if (m_program_handle) glDeleteProgram(m_program_handle);
}

//---------------------------------

GLuint Shader::getNativeHandle()
{
	return m_program_handle;
}

//---------------------------------

bool Shader::loadFromMemory(const std::string& source, Shader::Type type)
{
	if (type == Shader::Vertex) return compile(source.data(), nullptr);
	else if (type == Shader::Fragment) return compile(source.data(), nullptr);

	printf("Trying to load unknown type of shader (0x%02X)\n", type);
	return false;
}

bool Shader::loadFromMemory(const std::string& vertex_source, const std::string& fragment_source)
{
	return compile(vertex_source.data(), fragment_source.data());
}

//---------------------------------

bool Shader::loadFromStream(std::ifstream& stream, Shader::Type type)
{
	std::vector <char> buffer;
	ReadStream(stream, &buffer);
	return loadFromMemory(buffer.data(), type);
}

bool Shader::loadFromStream(std::ifstream& vertex_stream, std::ifstream& fragment_stream)
{
	std::vector <char> vertex_buffer;
	ReadStream(vertex_stream, &vertex_buffer);

	std::vector <char> fragment_buffer;
	ReadStream(fragment_stream, &fragment_buffer);

	return loadFromMemory(vertex_buffer.data(), fragment_buffer.data());
}

//---------------------------------

bool Shader::loadFromFile(const std::filesystem::path& filename, Shader::Type type)
{
	std::ifstream stream;
	stream.open(filename, std::ios::binary);
	if (!stream)
	{
		printf("Failed to open shader source '%s'\n", filename.string().c_str());
		return false;
	}

	bool result = loadFromStream(stream, type);

	stream.close();
	return result;
}

bool Shader::loadFromFile(const std::filesystem::path& vertex_filename, const std::filesystem::path& fragment_filename)
{
	std::ifstream vertex_stream;
	vertex_stream.open(vertex_filename);
	if (!vertex_stream)
	{
		printf("Failed to load vertex shader source '%s'\n", vertex_filename.string().c_str());
		return false;
	}

	std::ifstream fragment_stream;
	fragment_stream.open(fragment_filename);
	if (!fragment_stream)
	{
		vertex_stream.close();
		printf("Failed to load fragment shader source '%s'\n", fragment_filename.string().c_str());
		return false;
	}

	bool result = loadFromStream(vertex_stream, fragment_stream);

	vertex_stream.close();
	fragment_stream.close();
	return result;
}

//---------------------------------

bool Shader::compile(const char* vertex_source, const char* fragment_source)
{
	// Deleting previously generated shader program
	if (m_program_handle) glDeleteProgram(m_program_handle);

	// Creating new programe
	m_program_handle = glCreateProgram();

	static GLchar info[512] = "";
	static GLint  success = 0;

	if (vertex_source)
	{
		// Creating and compiling shader
		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
		glCompileShader(vertex_shader);

		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex_shader, sizeof(info), NULL, info);
			printf("Vertex shader compilation failed:\n%s\n", info);

			glDeleteShader(vertex_shader);
			return false;
		}

		// Attaching to program and deleting shader, it's no mode needed
		glAttachShader(m_program_handle, vertex_shader);
		glDeleteShader(vertex_shader);
	}

	if (fragment_source)
	{
		// All same with the fragment shader
		GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
		glCompileShader(fragment_shader);

		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment_shader, sizeof(info), NULL, info);
			printf("Fragment shader compilation failed:\n%s\n", info);

			glDeleteShader(fragment_shader);
			return false;
		}

		glAttachShader(m_program_handle, fragment_shader);
		glDeleteShader(fragment_shader);
	}

	// Linking program and checking result
	glLinkProgram(m_program_handle);
	glGetProgramiv(m_program_handle, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_program_handle, sizeof(info), nullptr, info);
		printf("Shader program linking failed:\n%s\n", info);

		return false;
	}

	return true;
}

GLint Shader::getUniformLocation(const char* name)
{
	GLint location = glGetUniformLocation(m_program_handle, name);
	if (location == -1)
		printf("Uniform '%s' not found in shader\n", name);

	return location;
}

//---------------------------------

void Shader::use()
{
	glUseProgram(m_program_handle);
}

//---------------------------------

void Shader::setUniform(const char* name, const glm::mat4& matrix)
{
	GLint location = getUniformLocation(name);
	if (location != -1)
		glProgramUniformMatrix4fv(m_program_handle, location, 1, GL_FALSE, glm::value_ptr(matrix)); glcheck;
}

void Shader::setUniform(const char* name, const glm::vec3& vector)
{
	GLint location = getUniformLocation(name);
	if (location != -1)
		glProgramUniform3f(m_program_handle, location, vector.x, vector.y, vector.z); glcheck;
}

void Shader::setUniform(const char* name, const glm::vec4& vector)
{
	GLint location = getUniformLocation(name);
	if (location != -1)
		glProgramUniform4f(m_program_handle, location, vector.x, vector.y, vector.z, vector.w); glcheck;
}

void Shader::setUniform(const char* name, const Color& color)
{
	GLint location = getUniformLocation(name);
	if (location != -1)
		glProgramUniform4f(m_program_handle, location, color.r, color.g, color.b, color.a); glcheck;
}

void Shader::setUniform(const char* name, int value)
{
	GLint location = getUniformLocation(name);
	if (location != -1)
		glProgramUniform1i(m_program_handle, location, value); glcheck;
}

void Shader::setUniform(const char* name, float value)
{
	GLint location = getUniformLocation(name);
	if (location != -1)
		glProgramUniform1f(m_program_handle, location, value); glcheck;
}

//---------------------------------

size_t ReadStream(std::ifstream& stream, std::vector <char>* buffer)
{
	stream.seekg(0, std::ios::end);

	std::streamoff file_size = stream.tellg();
	if (file_size)
	{
		stream.seekg(0, std::ios::beg);
		buffer->resize(static_cast <unsigned> (file_size));
		stream.read(buffer->data(), file_size);
		buffer->push_back('\0');
	}

	return buffer->size();
}

//---------------------------------