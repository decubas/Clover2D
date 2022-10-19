#pragma once

#include <Clover.h>

	class FileDialogs
	{
	public:
		// These return empty strings if cancelled
 		static std::optional<std::string> OpenFile(const char* filter);
 		static std::optional<std::string> SaveFile(const char* filter);
 		static std::string GetFileName(const std::string filepath);
	};
