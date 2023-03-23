#include "Utils.hpp"
#include <Windows.h>
#include <filesystem>
#include <fstream>

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

void glSetEnabled(GLenum index, bool state)
{
	if (state) glEnable(index);
	else       glDisable(index);
}

//-----------------------------------

size_t ReadStream(std::ifstream& stream, std::vector <char>* buffer)
{
	stream.seekg(0, std::ios::end);

	std::streamoff file_size = stream.tellg();
	if (file_size)
	{
		stream.seekg(0, std::ios::beg);
		buffer->resize(static_cast<unsigned>(file_size));
		stream.read(buffer->data(), file_size);
		buffer->push_back('\0');
	}

	return buffer->size();
}

size_t ReadStreamBinary(std::ifstream& stream, std::vector<uint8_t>* buffer)
{
	stream.seekg(0, std::ios::end);
	std::streamoff file_size = stream.tellg();
	if (file_size)
	{
		stream.seekg(0, std::ios::beg);
		buffer->resize(static_cast<unsigned>(file_size));
		stream.read(reinterpret_cast<char*>(buffer->data()), file_size);
	}

	return buffer->size();
}

//-----------------------------------

const char* StrPBRK(const char* begin, const char* delimiters, const char* limit /*= nullptr*/)
{
	for (const char* str = begin; *str; str++)
		if ((limit && str >= limit) || strchr(delimiters, *str)) return str;

	return nullptr;
}

const char* StrPBRKI(const char* begin, const char* delimiters, const char* limit /*= nullptr*/)
{
	for (const char* str = begin; *str; str++)
		if ((limit && str >= limit) || !strchr(delimiters, *str)) return str;

	return nullptr;
}

const char* StrPBRK2(const char* begin, const char* delimiters1, const char* delimiters2, const char* limit /*= nullptr*/)
{
	for (const char* str = begin; *str; str++)
		if ((limit && str >= limit) || (strchr(delimiters1, *str) || strchr(delimiters2, *str))) return str;

	return nullptr;
}

bool IsFloatStr(const char* begin, const char* limit /*= nullptr*/)
{
	constexpr const char* allowed_chars = ".-0123456789";
	for (const char* str = begin; *str && (!limit || str < limit); str++)
		if (!strchr(allowed_chars, *str)) return false;

	return true;
}

//-----------------------------------

namespace std
{

std::string to_string(const glm::vec3& vector)
{
	static char buffer[BUFFSIZE] = "";
	sprintf_s(buffer, BUFFSIZE, "{%.2f, %.2f, %.2f}", vector.x, vector.y, vector.z);
	return std::string(buffer);
}

} // namespace std

//-----------------------------------