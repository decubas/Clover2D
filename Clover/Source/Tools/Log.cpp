#include "Clover.h"

#include <Tools/Log.h>

		static TCHAR lpTimeStr[256];

		void _prepare_print(WORD color, const char* type, const char* filename, int line)
		{
			GetTimeFormat(LOCALE_SYSTEM_DEFAULT, TIME_FORCE24HOURFORMAT, NULL, L"hh:mm:ss", lpTimeStr, 256 * sizeof(TCHAR));
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
			printf("%s (line %d)\n", filename, line);
			std::wcout << "[" << lpTimeStr << "] " << type << " ->";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
		}

		void Log::_Log(const char* filename, int line, const char* format, ...)
		{
			_prepare_print(FOREGROUND_BLUE | FOREGROUND_GREEN, "Log", filename, line);

			va_list args;
			va_start(args, format);
			vprintf(format, args);
			va_end(args);

			printf("\n");
		}

		void Log::_Info(const char* filename, int line, const char* format, ...)
		{
			_prepare_print(FOREGROUND_GREEN, "Info", filename, line);

			va_list args;
			va_start(args, format);
			vprintf(format, args);
			va_end(args);

			printf("\n");
		}

		void Log::_Warning(const char* filename, int line, const char* format, ...)
		{
			_prepare_print(FOREGROUND_GREEN | FOREGROUND_RED, "Warning", filename, line);

			va_list args;
			va_start(args, format);
			vprintf(format, args);
			va_end(args);

			printf("\n");
		}

		void Log::_Error(const char* filename, int line, const char* format, ...)
		{
			_prepare_print(FOREGROUND_RED, "Error", filename, line);


			va_list args;
			va_start(args, format);
			vprintf(format, args);
			va_end(args);

			printf("\n");
		}

		void Log::_Assert(const char* filename, int line, int condition, const char* format, ...)
		{
			if (!condition)
			{
				_prepare_print(FOREGROUND_RED, "Assertion Failure", filename, line);

				va_list args;
				va_start(args, format);
				vprintf(format, args);
				va_end(args);

				printf("\n");

				assert(false);
			}
		}
