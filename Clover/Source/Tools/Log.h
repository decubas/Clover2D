#pragma once

#include <Clover.h>

#define Cl_DebugLog(format, ...) Log::_Log(__FUNCTION__, __LINE__, format, __VA_ARGS__)
#define Cl_Info(format, ...) Log::_Info(__FUNCTION__, __LINE__, format, __VA_ARGS__)
#define Cl_Warning(format, ...) Log::_Warning(__FUNCTION__, __LINE__, format, __VA_ARGS__)
#define Cl_Error(format, ...) Log::_Error(__FUNCTION__, __LINE__, format, __VA_ARGS__)
#define Cl_Assert(condition, format, ...) Log::_Assert(__FUNCTION__, __LINE__, condition, format, __VA_ARGS__)


	class Log
	{
	public:
		static void _Log(const char* filename, int line, const char* format, ...);
		static void _Info(const char* filename, int line, const char* format, ...);
		static void _Warning(const char* filename, int line, const char* format, ...);
		static void _Error(const char* filename, int line, const char* format, ...);
		static void _Assert(const char* filename, int line, int condition, const char* format, ...);
	};
