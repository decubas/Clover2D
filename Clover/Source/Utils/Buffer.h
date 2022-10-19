#pragma once

#include <Clover.h>

	class ElementBuffer
	{
		struct Element
		{
			void* Data = nullptr;
			u32 BytesSize = 0;

			template<class T>
			T* get()
			{
				return (T*)Data;
			}
		};

		Element* Elements;
		u32 BytesSize;
		u32 Count;
		u32 MaxCount;
	public:

		ElementBuffer()
			: Elements(nullptr), BytesSize(0), Count(0)
		{
		}

		ElementBuffer(u32 size)
			: MaxCount(size), BytesSize(size * sizeof(Element)), Count(0)
		{
			allocate(size);
		}

		inline uint32_t SizeInBytes() const { return Count * sizeof(Element); }
		inline uint32_t Size() const { return Count; }
		inline uint32_t MaxSize() const { return MaxCount; }

#pragma region Funtionality
		void copy(ElementBuffer& other)
		{
			allocate(other.MaxCount);
			memcpy(Elements, other.Elements, other.BytesSize);
			Count = other.Count;
		}

		void safeCopy(ElementBuffer& other)
		{
			allocate(other.MaxCount);

			for (byte i = 0; i < other.MaxCount; i++)
			{
				Element* e = get(i);
				e = createElement(other.Elements[i].Data, other.Elements[i].BytesSize);
			}

			Count = other.Count;
		}

		void allocate(u32 size)
		{
			if (Elements == nullptr) clear();

			if (size == 0)
				return;

			Elements = new Element[size];
			BytesSize = size * sizeof(Element);
		}

		void resize(u32 size)
		{
			allocate(size);
			memcpy(Elements, Elements, BytesSize);
		}

		void ZeroInitialize()
		{
			Count = 0;
			if (Elements)
				memset(Elements, 0, BytesSize);
		}

		template<typename T>
		T& read(u32 offset = 0)
		{
			return *(T*)(Elements + offset);
		}

		template<typename T>
		T* at(u32 offset = 0)
		{
			return (T*)(Elements + offset);
		}

		static u32 find(ElementBuffer* buf, void* target)
		{
			for (u32 offset = 0; offset < buf->MaxCount; offset++)
			{
				if (buf->Elements[offset].Data == target)
				{
					return offset;
				}
			}
			return -1;
		}

		void destroy(u32 offset)
		{
			delete Elements[offset].Data;
			Elements[offset].BytesSize = 0;
			organize(offset);
			Count--;
		}

		void* write(void* data, u32 byteSize, u32 offset = 0)
		{
			checkIfResize();

			Element* e = Elements + offset;
			e = createElement(data, byteSize);

			Count++;
			return e->Data;
		}

		void* writeBack(void* data, u32 byteSize)
		{

			checkIfResize();

			Element* e = Elements + Count; 
			e = createElement(data, byteSize);


			Count++;
			return e->Data;
		}

		template<class T>
		T* emplaceBack(T* data, u32 byteSize)
		{

			checkIfResize();

			Element* e = Elements + Count;
			e->Data = (void*)data;
			e->BytesSize = byteSize;

			Count++;
			return (T*)e->Data;
		}
		
		void* moveBack(void* data, u32 byteSize)
		{

			checkIfResize();

			Element* e = Elements + Count;
			e->Data = std::move(data);
			e->BytesSize = byteSize;

			Count++;
			return e->Data;
		}

		void clear()
		{
			delete[] Elements;
			Elements = nullptr;
		}

		operator bool() const
		{
			return Elements != nullptr;
		}

		void* at(int index)
		{
			return Elements[index].Data;
		}

		template<class T>
		T* at(int index)
		{
			return (T*)Elements[index].Data;
		}

		void* operator[](int index)
		{
			return Elements[index].Data;
		}

		void* operator[](int index) const
		{
			return Elements[index].Data;
		}


		template<class T, typename F>
		void foreach(F&  func)
		{
			for (u32 offset = 0; offset < Count; offset++)
			{
				func((T*)Elements[offset].Data);
			}
		}

		template<class T, typename F>
		void foreachBool(F& func)
		{
			for (u32 offset = 0; offset < Count; offset++)
			{
				if (func((T*)Elements[offset].Data))
				{
					return;
				}
			}
		}

#pragma endregion Funtionality

#pragma region Iterator

		struct Iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;

			using value_type = Element;
			using pointer = value_type*;  // or also value_type*
			using reference = value_type&;  // or also value_type&

			Iterator(pointer ptr) : m_ptr(ptr) {}

			reference operator*() const { return *m_ptr; }
			pointer operator->() { return m_ptr; }

			// Prefix increment
			Iterator& operator++() { m_ptr++; return *this; }

			// Postfix increment
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

			friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
			friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

		private:

			pointer m_ptr;
		};

		Iterator begin() { return Iterator(Elements); }
		Iterator end() { return Iterator(&Elements[Count]); }
#pragma endregion Iterator

	private:

		void checkIfResize()
		{
			if (Count * sizeof(Element) >= BytesSize)
			{
				resize((u32)((float)BytesSize * 1.5f));
			}
		}

		Element* get(u32 index)
		{
			return &Elements[index];
		}

		void organize(u32 index)
		{
			Element* current = get(index);

			for (u32 i = index+1; i < Count; i++)
			{
				Element* next = get(i);

				current->Data = next->Data;
				current->BytesSize = next->BytesSize;
				
				next->Data = nullptr;
				next->BytesSize = 0;

				current = next;
			}

		}

		Element* createElement(void* data, u32 bytesSize)
		{
			Element* e = new Element();
			memcpy(e->Data, data, bytesSize);
			e->BytesSize = bytesSize;
			return e;
		}
	};
