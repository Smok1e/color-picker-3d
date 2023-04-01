#pragma once

#include <varargs.h>

//-----------------------------------

#define LogInfo(format, ...)    Log(LoggingLevel::Info,    __FILE__, __LINE__, format, __VA_ARGS__);
#define LogWarning(format, ...) Log(LoggingLevel::Warning, __FILE__, __LINE__, format, __VA_ARGS__);
#define LogError(format, ...)   Log(LoggingLevel::Error,   __FILE__, __LINE__, format, __VA_ARGS__);

#ifdef _DEBUG
	#define LogAssert(expr) if (!(expr)) LogError("Debug assertion failed: %s was false", #expr);
#else
	// Disabled in release build
	#define LogAssert(expr) ;
#endif

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
	LOGGING_PRINT_LOCATION = 0x02,
	LOGGING_FLAGS_DEFAULT = LOGGING_INSERT_NEWLINE | LOGGING_PRINT_LOCATION
};

//-----------------------------------

void Log(LoggingLevel level, const char* file, int line, const char* format, ...);
void LogV(LoggingLevel level, const char* file, int line, const char* format, va_list args);

// Sets current logging level
void SetLoggingLevel(LoggingLevel level);  

// Returns current logging level
LoggingLevel GetLoggingLevel();            

 // Sets current logging flags
void SetLoggingFlags(LoggingFlags flags);

// Returns current logging flags
LoggingFlags GetLoggingFlags();

//-----------------------------------