#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <cstdio>
#include <string>
#include <vector>

//-----------------------------------

constexpr size_t BUFFSIZE = 1024;

#if defined(_DEBUG) || defined(FORCE_GL_CHECK)
	#define glcheck CheckOpenGLError(__FILE__, __FUNCSIG__, __LINE__);
#endif

//-----------------------------------

#define inspect(expression) Inspect(#expression, expression)

void CheckOpenGLError(const char* file, const char* func, int line);
void GetOpenGLErrorMessage(GLenum code, const char** name, const char** description);
void glSetEnabled(GLenum index, bool state);

//-----------------------------------

// std::strpbrk alternative
const char* StrPBRK(const char* begin, const char* delimiters, const char* limit = nullptr);						 

// Inverted strpbrk
const char* StrPBRKI(const char* begin, const char* delimiters, const char* limit = nullptr);

// An strpbrk overloading that accepts two delimiter strings
const char* StrPBRK2(const char* begin, const char* delimiters1, const char* delimiters2, const char* limit = nullptr);

// Returns true if the string could be parsed as float number
bool IsFloatStr(const char* begin, const char* limit = nullptr);

// Reads all content from stream into a vector
size_t ReadStream      (std::ifstream& stream, std::vector<char>*    buffer);
size_t ReadStreamBinary(std::ifstream& stream, std::vector<uint8_t>* buffer);

//-----------------------------------

template <typename T>
T Inspect(const char* expression, const T& value);

namespace std { std::string to_string(const glm::vec3& vector); }

//-----------------------------------

template <typename T>
T Inspect(const char* expression, const T& value)
{
	printf("[%-100s] = %s\n", expression, std::to_string(value).c_str());
	return value;
}

//-----------------------------------