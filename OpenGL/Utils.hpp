#pragma once

#include <GL/glew.h>
#include <cstdio>
#include <string>

//-----------------------------------

#if defined(_DEBUG) || defined(FORCE_GL_CHECK)
	#define glcheck CheckOpenGLError(__FUNCSIG__, __LINE__);
#endif

#define inspect(expression) Inspect(#expression, expression)

void CheckOpenGLError(const char* func, int line);
void GetOpenGLErrorMessage(GLenum code, const char** name, const char** description);
void glSetEnabled(GLenum index, bool state);

template <typename T>
T Inspect(const char* expression, const T& value);

//-----------------------------------

template <typename T>
T Inspect(const char* expression, const T& value)
{
	printf("[%-100s] = %s\n", expression, std::to_string(value).c_str());
	return value;
}

//-----------------------------------