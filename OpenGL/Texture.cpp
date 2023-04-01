#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>

#include "Texture.hpp"
#include "Utils.hpp"
#include "Logging.hpp"

//---------------------------------

Texture::Texture():
	m_texture_handle(0)
{
	glSafeCallVoid(glGenTextures(1, &m_texture_handle));
}

Texture::Texture(const std::filesystem::path& filename):
	Texture()
{
	loadFromFile(filename);
}

Texture::~Texture()
{
	glSafeCallVoid(glDeleteTextures(1, &m_texture_handle));
}

//---------------------------------

bool Texture::loadFromFile(const std::filesystem::path& filename)
{
	if (!std::filesystem::exists(filename))
	{
		LogError("Failed to load texture '%s': No such file", filename.string().c_str());
		return false;
	}

	if (std::filesystem::is_directory(filename))
	{
		LogError("Failed to load texture '%s': This is a directory", filename.string().c_str());
		return false;
	}

	// Loading image
	int size_x = 0;
	int size_y = 0;
	int channels = 0;
	uint8_t* texture_data = stbi_load(filename.string().c_str(), &size_x, &size_y, &channels, STBI_rgb_alpha);
	if (!texture_data)
	{
		LogError("Failed to load texture '%s': %s\n", filename.string().c_str(), stbi_failure_reason());

		// Generating red texture that indicates loading failure
		static uint32_t pixel = 0xFF0000FF;

		bind();
		glSafeCallVoid(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixel));

		return false;
	}

	// Copying image data into texture video memory
	bind();
	glSafeCallVoid(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size_x, size_y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data));
	glSafeCallVoid(glGenerateMipmap(GL_TEXTURE_2D));

	// Deleting image data
	stbi_image_free(texture_data);

	return true;
}

bool Texture::saveToFile(const std::filesystem::path& filename) const
{
	if (std::filesystem::is_directory(filename))
	{
		LogError("Can't save texture to '%s': File is a directory", filename.string().c_str());
		return false;
	}

	constexpr int compression_level = 4;

	glm::uvec2 size = getSize();
	uint8_t* data = new uint8_t[size.x*size.y*4];

	bind();
	glSafeCallVoid(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

	std::filesystem::path extension = filename.extension();
	
	int result = 0;

	if      (extension == ".jpg") result = stbi_write_jpg(filename.string().c_str(), size.x, size.y, compression_level, data, 100);
	else if (extension == ".bmp") result = stbi_write_bmp(filename.string().c_str(), size.x, size.y, compression_level, data);
	else if (extension == ".tga") result = stbi_write_tga(filename.string().c_str(), size.x, size.y, compression_level, data);

	else
	{
		if (extension != ".png")
			LogWarning("Unknown extension '%s'; Saving image as png", extension.string().c_str());

		result = stbi_write_png(filename.string().c_str(), size.x, size.y, compression_level, data, 0);
	}

	delete[] data;

	if (!result)
	{
		LogError("Failed to save texture '%s'", filename.string().c_str());
		return false;
	}

	return true;
}

//---------------------------------

void Texture::captureFrameBuffer()
{
	GLint dims[4] = {0};
	glSafeCallVoid(glGetIntegerv(GL_VIEWPORT, dims));

	bind();
	glSafeCallVoid(glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, dims[2], dims[3], 0));

	flipVertically();
}

//---------------------------------

void Texture::flipVertically()
{
	glm::uvec2 size = getSize();
	uint8_t* data = new uint8_t[size.x*size.y*4];

	// Transferring data from GPU to RAM
	bind();
	glSafeCallVoid(glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));

	// For security purposes, this part of code will be obscured & undocumented
	for (size_t y = 0; y < size.y/2; y++)
		for (size_t x = 0; x < size.x*4; x++) 
			std::swap(*(data+y*size.x*4+x), *(data+(size.y-y-1)*size.x*4+x));

	// Returning data back to GPU
	glSafeCallVoid(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	delete[] data;
}

//---------------------------------

GLuint Texture::getNativeHandle() const
{
	return m_texture_handle;
}

glm::uvec2 Texture::getSize() const
{
	GLint size_x = 0;
	GLint size_y = 0;
	glSafeCallVoid(glGetTextureLevelParameteriv(m_texture_handle, 0, GL_TEXTURE_WIDTH, &size_x));
	glSafeCallVoid(glGetTextureLevelParameteriv(m_texture_handle, 0, GL_TEXTURE_HEIGHT, &size_y));
	return glm::u32vec2(size_x, size_y);
}

unsigned Texture::getSizeX() const
{
	GLint size_x = 0;
	glSafeCallVoid(glGetTextureLevelParameteriv(m_texture_handle, 0, GL_TEXTURE_WIDTH, &size_x));
	return size_x;
}

unsigned Texture::getSizeY() const
{
	GLint size_y = 0;
	glSafeCallVoid(glGetTextureLevelParameteriv(m_texture_handle, 0, GL_TEXTURE_HEIGHT, &size_y));
	return size_y;
}

float Texture::getRatio() const
{
	return static_cast<float>(getSizeX())/static_cast<float>(getSizeY());
}

//---------------------------------

void Texture::bind() const
{
	glSafeCallVoid(glBindTexture(GL_TEXTURE_2D, m_texture_handle));
}

//---------------------------------

void Texture::setMinifierFilter(Texture::Filter filter)
{
	glSafeCallVoid(glTextureParameteri(m_texture_handle, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter)));
}

void Texture::setMagnifierFilter(Texture::Filter filter)
{
	glSafeCallVoid(glTextureParameteri(m_texture_handle, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter)));
}

void Texture::setFilters(Texture::Filter minifier, Texture::Filter magnifier)
{
	setMinifierFilter (minifier );
	setMagnifierFilter(magnifier);
}

void Texture::setFilters(Texture::Filter filter)
{
	setFilters(filter, filter);
}

void Texture::setWrappingMode(Texture::WrappingMode mode)
{
	glSafeCallVoid(glTextureParameteri(m_texture_handle, GL_TEXTURE_WRAP_S, static_cast<GLint>(mode)));
	glSafeCallVoid(glTextureParameteri(m_texture_handle, GL_TEXTURE_WRAP_T, static_cast<GLint>(mode)));
}

Texture::WrappingMode Texture::getWrappingMode() const
{
	GLint mode = 0;
	glGetTextureParameteriv(m_texture_handle, GL_TEXTURE_WRAP_S, &mode);
	return static_cast<Texture::WrappingMode>(mode);
}

//---------------------------------
