#pragma once

#include <varargs.h>

//-----------------------------------

#define LogInfo(format, ...)    Log(LoggingLevel::Info,    __FILE__, __LINE__, format, __VA_ARGS__);
#define LogWarning(format, ...) Log(LoggingLevel::Warning, __FILE__, __LINE__, format, __VA_ARGS__);
#define LogError(format, ...)   Log(LoggingLevel::Error,   __FILE__, __LINE__, format, __VA_ARGS__);

//-----------------------------------

enum class LoggingLevel
{
	Info,
	Warning,
	Error,
	Silent,

	DefaultDebug = Warning,
	DefaultRelease = Error,

	#ifdef _DEBUG
		Default = DefaultDebug
	#else
		Default = DefaultRelease
	#endif
};

enum LoggingFlags
{
	LOGGING_INSERT_NEWLINE = 0x01,
	LOGGING_FLAGS_DEFAULT = LOGGING_INSERT_NEWLINE
};

//-----------------------------------

void Log(LoggingLevel level, const char* file, int line, const char* format, ...);
void LogV(LoggingLevel level, const char* file, int line, const char* format, va_list args);

void SetLoggingLevel(LoggingLevel level);
LoggingLevel GetLoggingLevel();

void SetLoggingFlags(LoggingFlags flags);
LoggingFlags GetLoggingFlags();

//-----------------------------------