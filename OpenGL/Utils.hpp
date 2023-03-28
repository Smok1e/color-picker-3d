#pragma once

#ifdef APIENTRY
	#undef APIENTRY
#endif

#include <Windows.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <cstdio>
#include <string>
#include <vector>

//-----------------------------------

constexpr size_t BUFFSIZE = 1024;

//-----------------------------------

template<typename T>
T    CheckOpenGLCall(T expr_result, const char* expression, const char* file, int line);
void CheckOpenGLCall(               const char* expression, const char* file, int line);
void GetOpenGLErrorMessage(GLenum code, const char** name, const char** description);
void glSetEnabled(GLenum index, bool state);

//-----------------------------------

#if defined(_DEBUG) || defined(FORCE_GL_CHECK)
	// Checks for opengl error after call of some opengl function
	#define glSafeCall(expression) CheckOpenGLCall(expression, #expression, __FILE__, __LINE__)

	// Same as glSafeCall but for functions that returns void
	#define glSafeCallVoid(expression) expression; CheckOpenGLCall(#expression, __FILE__, __LINE__)
#else
	// Disabled in release build
	#define glSafeCall(expression) expression

	// Disabled in release build
	#define glSafeCallVoid(expression) expression
#endif

#define inspect(expression) Inspect(#expression, expression)

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

template<typename T>
T CheckOpenGLCall(T expr_result, const char* expression, const char* file, int line)
{
	CheckOpenGLCall(expression, file, line);
	return expr_result;
}

//-----------------------------------