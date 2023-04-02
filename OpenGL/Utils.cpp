#include <filesystem>
#include <fstream>
#include <conio.h>

#include "Utils.hpp"
#include "Logging.hpp"

//-----------------------------------

void CheckOpenGLCall(const char* expression, const char* file, int line)
{
	GLenum code = glGetError();
	if (code != GL_NO_ERROR)
	{
		const char* name = nullptr;
		const char* description = nullptr;
		GetOpenGLErrorMessage(code, &name, &description);
		Log(LoggingLevel::Error, file, line, "%s caused %s", expression, name);
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
			"Given when an enumeration parameter is not a legal enumeration for that function.\n"
			"This is given only for local problems; if the spec allows the enumeration in certain\n"
			"circumstances, where other parameters or state dictate those circumstances,\n"
			"then GL_INVALID_OPERATION is the result instead."
		);

		__case(
			GL_INVALID_VALUE,
			"Given when a value parameter is not a legal value for that function. This is only\n"
			"given for local problems; if the spec allows the value in certain circumstances,\n"
			"where other parameters or state dictate those circumstances, then GL_INVALID_OPERATION\n"
			"is the result instead."
		);

		__case(
			GL_INVALID_OPERATION,
			"Given when the set of state for a command is not legal for the parameters given to that\n"
			"command. It is also given for commands where combinations of parameters define what the\n"
			"legal parameters are."
		)

			__case(
				GL_STACK_OVERFLOW,
				"Given when a stack pushing operation cannot be done because it would overflow the limit\n"
				"of that stack's size."
			)

			__case(
				GL_STACK_UNDERFLOW,
				"Given when a stack popping operation cannot be done because the stack is already at its\n"
				"lowest point."
			);

		__case(
			GL_OUT_OF_MEMORY,
			"Given when performing an operation that can allocate memory, and the memory cannot be\n"
			"allocated. The results of OpenGL functions that return this error are undefined; it\n"
			"is allowable for partial execution of an operation to happen in this circumstance."
		);

		__case(
			GL_INVALID_FRAMEBUFFER_OPERATION,
			"Given when doing anything that would attempt to read from or write/render to a\n"
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

int SystemF(const char* format, ...)
{
	char buffer[BUFFSIZE] = "";
	va_list args = {};
	va_start(args, format);
	vsprintf_s(buffer, format, args);
	va_end(args);

	return system(buffer);
}

const char* FormatTmp(const char* format, ...)
{
	static char buffer[BUFFSIZE] = "";
	va_list args = {};
	va_start(args, format);
	vsprintf_s(buffer, format, args);
	va_end(args);

	return buffer;
}

//-----------------------------------