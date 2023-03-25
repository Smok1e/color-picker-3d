#pragma once

#include <gl/glew.h>
#include <filesystem>
#include <glm/glm.hpp>

//---------------------------------

// Simple class that represents OpenGL texture data in GPU memory
class Texture
{
public:
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

protected:
	GLuint m_texture_handle;

};

//---------------------------------