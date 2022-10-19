#include "Clover.h"

#include <Tools/FileManager.h>


	FileManager::FileHandle* FileManager::Open(const char* file, uint8_t mode)
	{
		FileHandle* handle = new FileHandle();

		handle->file_.open(file, mode);
		if (!handle->file_)
		{
			handle->file_.close();

			std::cout << "Creating File" << std::endl;
			std::fstream f;
			f.open(file, FileManager::read);
			if (f)
			{
				std::cout << file << " Created" << std::endl;
			}
			f.close();
			handle->file_.open(file, mode);
		}
		handle->filepath_ = file;
		handle->mode_ = mode;

		return handle;
	}

	void FileManager::Close(FileHandle* handle)
	{
		CHECK(handle);
		handle->file_.close();
		delete handle;
	}

	void FileManager::WriteLine(FileHandle* handle, const char* data, int32_t line)
	{
		////////////////////////// CHECK
		CHECK(handle);

		std::string strLine;
		int32_t i = 0;
		handle->file_.seekp(std::ios::beg);		
		while (std::getline(handle->file_, strLine))
		{
			if (i == line)
			{
				WriteLine(handle, data);
			}
			++i;
		}
	}

	void FileManager::WriteLine(FileHandle* handle, const char* data)
	{
		CHECK(handle);
		handle->file_ << data << std::endl;
	}


	void FileManager::Write(FileHandle* handle, const char* data)
	{
		CHECK(handle);
		handle->file_ << data;
	}

	void FileManager::Write(FileHandle* handle, const int* data, uint32_t count /*= 1*/, const char* sepatation /*= ""*/)
	{
		Write<int>(handle, data, count, sepatation);
	}

	void FileManager::Write(FileHandle* handle, const float* data, uint32_t count /*= 1*/, const char* sepatation /*= ""*/)
	{
		Write<float>(handle, data, count, sepatation);
	}

	int32_t FileManager::Find(FileHandle* handle, const char* data, int32_t size)
	{
		CHECK(handle, -1);
		char* line = new char[size];
		handle->file_.seekp(std::ios::beg);
		int32_t pos = 0;

		while (!handle->file_.eof())
		{
			handle->file_.get(line, size, '\n');
			if (std::strcmp(line, data) == 1)
			{
				return pos;
			}
		}
		return -1;
	}

	std::string FileManager::ReadLine(FileHandle* handle, int32_t line)
	{
		CHECK(handle, "");
		std::string strLine;
		int32_t i = 0;
		handle->file_.seekp(std::ios::beg);
		while (std::getline(handle->file_, strLine))
		{
			if (line == i)
			{
				break;
			}
			++i;
		}
		return strLine;
	}

	std::string FileManager::ReadAll(FileHandle* handle)
	{
		CHECK(handle, "");
		std::string file = "";
		std::string strLine;
		handle->file_.seekp(std::ios::beg);
		while (std::getline(handle->file_, strLine))
		{
			file += strLine;
		}
		return file;
	}

	void FileManager::Read(FileHandle* handle, char* outBuffer, uint32_t count)
	{
		CHECK(handle);
		handle->file_.read(outBuffer, sizeof(char) * count);
	}

	uint32_t FileManager::Size(FileHandle* handle)
	{
		CHECK(handle, -1);
		handle->file_.seekp(std::ios::end);
		uint32_t size = handle->file_.tellg();
		handle->file_.seekp(std::ios::beg);
		return size;
	}
