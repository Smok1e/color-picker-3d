#pragma once

#include <gl/glew.h>
#include <filesystem>
#include <glm/glm.hpp>

//---------------------------------

// Simple class that represents OpenGL texture data in GPU memory
class Texture
{
public:
	enum class Filter {
		NearestNeightbour = GL_NEAREST,
		LinearInterpolation = GL_LINEAR
	};

	Texture();
	~Texture();

	// Loads texture from file
	bool loadFromFile(const std::filesystem::path& filename);

	// Returns native OpenGL texture handle
	GLuint getNativeHandle() const;

	glm::u32vec2 getSize() const;
	unsigned getSizeX() const;
	unsigned getSizeY() const;

	// returns width/height
	float getRatio() const;

	// Binds texture
	void bind() const;

	// Setting OpenGL texture instance parameters
	void setParameter(GLenum name, GLfloat parameter);
	void setParameter(GLenum name, const GLfloat* parameters);
	void setParameter(GLenum name, GLint parameter);
	void setParameter(GLenum name, const GLint* parameters);

	void setMinifierFilter(Filter filter);
	void setMagnifierFilter(Filter filter);
	void setFilters(Filter minifier, Filter magnifier);
	void setFilters(Filter filter);

protected:
	GLuint m_texture_handle;

};

//---------------------------------