#include "Clover.h"

#include <Tools/CommandList.h>

 
	DisplayList::DisplayList()
	{
		m_CommandBuffer = new u8[m_Size]; // 10mb buffer
		m_CommandBufferPtr = m_CommandBuffer;
		memset(m_CommandBuffer, 0, m_Size);
	}

	DisplayList::~DisplayList()
	{
		delete[] m_CommandBuffer;
	}

	void* DisplayList::Allocate(RenderCommandFn fn, u32 size)
	{
		// TODO: alignment
		*(RenderCommandFn*)m_CommandBufferPtr = fn;
		m_CommandBufferPtr += sizeof(RenderCommandFn);

		*(u32*)m_CommandBufferPtr = size;
		m_CommandBufferPtr += sizeof(u32);

		void* memory = m_CommandBufferPtr;
		m_CommandBufferPtr += size;

		m_CommandCount++;
		return memory;
	}


	void DisplayList::Copy(DisplayList& other)
	{
		memcpy(m_CommandBuffer, other.m_CommandBuffer, m_Size);
		m_CommandCount = other.m_CommandCount;
	}


	void DisplayList::Clear()
	{
		//memset(m_CommandBuffer, 0, m_Size);
		m_CommandBufferPtr = m_CommandBuffer;
		m_CommandCount = 0;
	}

	void DisplayList::Execute()
	{

		byte* buffer = m_CommandBuffer;

		if (m_CommandCount == 0) return;

		for (u32 i = 0; i < m_CommandCount; i++)
		{
			RenderCommandFn function = *(RenderCommandFn*)buffer;
			buffer += sizeof(RenderCommandFn);

			u32 size = *(u32*)buffer;
			buffer += sizeof(u32);
			function(buffer);
			buffer += size;
		}
		Clear();
	}
