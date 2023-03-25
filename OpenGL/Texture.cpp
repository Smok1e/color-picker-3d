#include "Texture.hpp"
#include <SOIL/SOIL.h>

//---------------------------------

Texture::Texture():
	m_texture_handle(0)
{}

Texture::~Texture()
{
	if (m_texture_handle) glDeleteTextures(1, &m_texture_handle);
}

//---------------------------------

bool Texture::loadFromFile(const std::filesystem::path& filename)
{
	// Deleting previously generated texture
	if (m_texture_handle) glDeleteTextures(1, &m_texture_handle);

	// Loading image
	int size_x = 0;
	int size_y = 0;
	unsigned char* texture_data = SOIL_load_image(filename.string().c_str(), &size_x, &size_y, nullptr, SOIL_LOAD_RGBA);
	if (!texture_data)
	{
		printf("Failed to load texture '%s'", filename.string().c_str());
		return false;
	}

	// Generating texture
	glGenTextures(1, &m_texture_handle);
	if (!m_texture_handle)
	{
		printf("Failed to generate texture");
		return false;
	}

	// Copying image data into texture video memory
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size_x, size_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Deleting image data
	SOIL_free_image_data(texture_data);

	return true;
}

//---------------------------------

GLuint Texture::getNativeHandle() const
{
	return m_texture_handle;
}

glm::u32vec2 Texture::getSize() const
{
	GLint size_x = 0;
	GLint size_y = 0;
	glGetTextureLevelParameteriv(m_texture_handle, 0, GL_TEXTURE_WIDTH, &size_x);
	glGetTextureLevelParameteriv(m_texture_handle, 0, GL_TEXTURE_HEIGHT, &size_y);
	return glm::u32vec2(size_x, size_y);
}

unsigned Texture::getSizeX() const
{
	GLint size_x = 0;
	glGetTextureLevelParameteriv(m_texture_handle, 0, GL_TEXTURE_WIDTH, &size_x);
	return size_x;
}

unsigned Texture::getSizeY() const
{
	GLint size_y = 0;
	glGetTextureLevelParameteriv(m_texture_handle, 0, GL_TEXTURE_HEIGHT, &size_y);
	return size_y;
}

float Texture::getRatio() const
{
	return static_cast<float>(getSizeX())/static_cast<float>(getSizeY());
}

//---------------------------------

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_texture_handle);
}

//---------------------------------

void Texture::setParameter(GLenum name, GLfloat parameter)
{
	glTextureParameterf(m_texture_handle, name, parameter);
}

void Texture::setParameter(GLenum name, const GLfloat* parameters)
{
	glTextureParameterfv(m_texture_handle, name, parameters);
}

void Texture::setParameter(GLenum name, GLint parameter)
{
	glTextureParameteri(m_texture_handle, name, parameter);
}

void Texture::setParameter(GLenum name, const GLint* parameters)
{
	glTextureParameteriv(m_texture_handle, name, parameters);
}

//---------------------------------