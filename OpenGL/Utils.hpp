#pragma once

#include <GL/glew.h>
#include <cstdio>
#include <string>

//-----------------------------------

#define glcheck CheckOpenGLError(__FUNCSIG__, __LINE__);
#define inspect(expression) Inspect(#expression, expression)

void CheckOpenGLError(const char* func, int line);
void GetOpenGLErrorMessage(GLenum code, const char** name, const char** description);

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