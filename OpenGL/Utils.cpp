#include "Utils.hpp"
#include <Windows.h>

//-----------------------------------

void CheckOpenGLError(const char* func, int line)
{
	if (GLenum code = glGetError())
	{
		const char* name = nullptr;
		const char* description = nullptr;
		GetOpenGLErrorMessage(code, &name, &description);
		fprintf(stderr, "OpenGL error detected in function %s at line #%d: %s (0x%04X)\n%s\n", func, line, name, code, description);

		char text[2048] = "";
		sprintf_s(
			text, 
			"OpenGL error: %s (0x%04X)\n"
			"Detected in function %s at line #%d\n"
			"\n"
			"%s",
			name,
			code,
			func,
			line,
			description
		);

		MessageBoxA(nullptr, text, "Error", MB_ICONERROR | MB_OK);
	}
}

//-----------------------------------

void GetOpenGLErrorMessage(GLenum code, const char** pname, const char** pdescription)
{
	#define __case(enumeration, description) \
	case enumeration:                        \
		*pname = #enumeration; 			     \
		*pdescription = description; 		 \
		break;								 

	switch (code)
	{
		__case(
			GL_INVALID_ENUM,
			"Given when an enumeration parameter is not a legal enumeration for that function. " 
			"This is given only for local problems; if the spec allows the enumeration in certain "
			"circumstances, where other parameters or state dictate those circumstances, "
			"then GL_INVALID_OPERATION is the result instead."
		);

		__case(
			GL_INVALID_VALUE,
			"Given when a value parameter is not a legal value for that function. This is only "
			"given for local problems; if the spec allows the value in certain circumstances, "
			"where other parameters or state dictate those circumstances, then GL_INVALID_OPERATION "
			"is the result instead."
		);

		__case(
			GL_INVALID_OPERATION,
			"Given when the set of state for a command is not legal for the parameters given to that "
			"command. It is also given for commands where combinations of parameters define what the "
			"legal parameters are."
		)

		__case(
			GL_STACK_OVERFLOW,
			"Given when a stack pushing operation cannot be done because it would overflow the limit "
			"of that stack's size."
		)

		__case(
			GL_STACK_UNDERFLOW,
			"Given when a stack popping operation cannot be done because the stack is already at its "
			"lowest point."
		);

		__case(
			GL_OUT_OF_MEMORY,
			"Given when performing an operation that can allocate memory, and the memory cannot be "
			"allocated. The results of OpenGL functions that return this error are undefined; it "
			"is allowable for partial execution of an operation to happen in this circumstance."
		);

		__case(
			GL_INVALID_FRAMEBUFFER_OPERATION,
			"Given when doing anything that would attempt to read from or write/render to a "
			"framebuffer that is not complete."
		);

		__case(
			GL_CONTEXT_LOST,
			"Given if the OpenGL context has been lost, due to a graphics card reset"
		);

		default:
			*pname = "Unknown error";
			*pdescription = "Unknown description";
			break;
	}

	#undef __case
}

//-----------------------------------