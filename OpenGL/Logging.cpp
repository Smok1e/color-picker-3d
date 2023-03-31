#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <Windows.h>
#include <mutex>
#include <filesystem>

#include "Logging.hpp"

//------------------------

LoggingLevel _LoggingLevel = LoggingLevel::Default;
LoggingFlags _LoggingFlags = LoggingFlags::LOGGING_FLAGS_DEFAULT;
std::mutex _LoggingMutex;

//------------------------

bool _InitLogging()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	return true;
}

//------------------------

void Log(LoggingLevel level, const char* file, int line, const char* format, ...)
{
	va_list args = {};
	va_start(args, format);
	LogV(level, file, line, format, args);
	va_end(args);
}

void LogV(LoggingLevel level, const char* file, int line, const char* format, va_list args)
{
	if (level < _LoggingLevel) return;

	_LoggingMutex.lock();

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info = {};
	GetConsoleScreenBufferInfo(handle, &info);

	const char* prefix = nullptr;
	WORD attributes = 0;
	switch (level)
	{
		case LoggingLevel::Info:
			prefix = "INFO";
			attributes = FOREGROUND_GREEN | FOREGROUND_BLUE;
			break;

		case LoggingLevel::Warning:
			prefix = "WARNING";
			attributes = FOREGROUND_RED | FOREGROUND_GREEN;
			break;

		case LoggingLevel::Error:
			prefix = "ERROR";
			attributes = FOREGROUND_RED;
			break;

		default: 
			_LoggingMutex.unlock();
			return;
	}

	va_list args_copy = {};
	va_copy(args_copy, args);
	size_t size = vsnprintf(nullptr, 0, format, args_copy);
	va_end(args_copy);

	char* buffer = new char[size+1];
	vsprintf(buffer, format, args);

	SetConsoleTextAttribute(handle, attributes);
	printf("[%s] ", prefix);
	SetConsoleTextAttribute(handle, info.wAttributes);

	if (file)
	{
		std::filesystem::path filename = std::filesystem::path(file).filename();
		printf("[%s:%d] ", filename.string().c_str(), line);
	}

	printf(buffer);

	if (_LoggingFlags & LOGGING_INSERT_NEWLINE && buffer[size-1] != '\n')
		printf("\n");

	delete[] buffer;
	_LoggingMutex.unlock();
}

//------------------------

void SetLoggingLevel(LoggingLevel level)
{
	_LoggingLevel = level;
}

LoggingLevel GetLoggingLevel()
{
	return _LoggingLevel;
}

void SetLoggingFlags(LoggingFlags flags)
{
	_LoggingFlags = flags;
}

LoggingFlags GetLoggingFlags()
{
	return _LoggingFlags;
}

//------------------------

bool _LoggingInitialized = _InitLogging();

//------------------------