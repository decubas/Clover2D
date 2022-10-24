#pragma once

#include <vector>
#include <ostream>
#include <iostream>
#include <fstream>

#define CHECK(handle, value) if(handle == nullptr) return value;\
					  if(!handle->file_.is_open()) return value;

	class FileManager
	{
	public:
		static constexpr uint8_t write = (0x01);
		static constexpr uint8_t read = (0x02);
		static constexpr uint8_t read_write = (0x01 | 0x02);
		static constexpr uint8_t ate = (0x04);
		static constexpr uint8_t app = (0x08);
		static constexpr uint8_t trunc = (0x10);
		static constexpr uint8_t _Nocreate = (0x40);
		static constexpr uint8_t _Noreplace = (0x80);
		static constexpr uint8_t binary = (0x20);

		struct FileHandle
		{
			std::string filepath_;
			std::fstream file_;
			uint8_t mode_ = 0xff;

			void begining() { file_.seekp(std::ios::beg); }
			void end() { file_.seekp(std::ios::end); }
		};

		

		FileManager() = default;
		~FileManager() = default;

		static FileHandle* Open(const char* file, uint8_t mode = read_write);
		static void Close(FileHandle* handle);

		static void Write(FileHandle* handle, const char* data);
		static void WriteLine(FileHandle* handle, const char* data, int32_t line);
		static void WriteLine(FileHandle* handle, const char* data);


		static void Write(FileHandle* handle, const int* data, uint32_t count = 1, const char* separation = "");
		static void Write(FileHandle* handle, const float* data, uint32_t count = 1, const char* separation = "");

		template<class T>
		static void WriteBin(FileHandle* handle, const T* data, uint32_t count = 1);

		static int32_t Find(FileHandle* handle, const char* data, int32_t size);

		static std::string ReadLine(FileHandle* handle, int32_t line);
		static std::string ReadAll(FileHandle* handle);


		static void Read(FileHandle* handle, char* outBuffer, uint32_t count = 1);

		template<class T>
		static void ReadBin(FileHandle* handle, T* outBuffer, uint32_t count = 1);

		static uint32_t Size(FileHandle* handle);

	private:
		template<class T>
		static void Write(FileHandle* handle, const T* data, uint32_t count = 1, const char* separation = "");
	};



	template<class T>
	void FileManager::WriteBin(FileHandle* handle, const T* data, uint32_t count)
	{
		CHECK(handle, );
		handle->file_.write((char*)data, sizeof(T) * count);

		if (!handle->file_.good())
		{
			std::cout << "Error Writing" << std::endl;
		}
	}

	template<class T>
	void FileManager::ReadBin(FileHandle* handle, T* outBuffer, uint32_t count)
	{
		CHECK(handle, );
		handle->file_.read((char*)outBuffer, sizeof(T) * count);

		if (!handle->file_.good())
		{
			std::cout << "Error Reading" << std::endl;
		}
	}

	template<class T>
	void FileManager::Write(FileHandle* handle, const T* data, uint32_t count /*= 1*/, const char* separation /*= ""*/)
	{
		CHECK(handle, );
		for (int i = 0; i < count; ++i)
		{
			handle->file_ << *(data + i) << separation;
		}
	}
