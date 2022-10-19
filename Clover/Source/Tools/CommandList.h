#ifndef __DISPLAYLIST_H__
#define __DISPLAYLIST_H__ 1

#include <Clover.h>

	class DisplayList
	{
	public:
		typedef void(*RenderCommandFn)(void*);

		DisplayList();
		~DisplayList();

		void* Allocate(RenderCommandFn func, u32 size);

		void Copy(DisplayList& other);
		void Clear();

		void Execute();
	private:
		u8* m_CommandBuffer;
		u8* m_CommandBufferPtr;
		u32 m_CommandCount = 0;
		u32 m_Size = 10 * 1024 * 1024;
	};

#endif