#pragma once

#include <Clover.h>

	struct Handler
	{
	public:
		Handler()
		{
			handle_ = nullptr;
			size_ = 0;
		}

		Handler(void* ptr, size_t size)
		{
			if (size == 0 || ptr == nullptr) return;
			Alloc(ptr, size);
		}
		Handler(const Handler& other)
		{
			if (other.handle_ == nullptr || other.size_ == 0) return;
			if (handle_) free(handle_);
			Alloc(other.handle_, other.size_);
		}
		~Handler()
		{
			Free();
		}
		template<class T>
		T* get()
		{

			return (T*)handle_;
		}
		void Alloc(void* ptr, size_t size)
		{
			handle_ = malloc(size);
			size_ = size;
			memcpy(handle_, ptr, size_);
		}
		void Move(void* ptr, size_t size)
		{
			handle_ = malloc(size);
			size_ = size;
			handle_ = std::move(ptr);
		}
		void Free()
		{
			if (size_ == 0 || handle_ == nullptr) return;
			free(handle_);
			handle_ = nullptr;
			size_ = 0;
		}

	private:
		void* handle_ = nullptr;
		size_t size_ = 0;
	};
