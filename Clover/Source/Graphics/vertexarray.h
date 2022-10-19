#ifndef _BASE_VERTEXARRAY_H__
#define _BASE_VERTEXARRAY_H__

#include <Clover.h>

#include "buffer.h"


	class VertexArray : public Cl_Object
	{
	public:
		VertexArray() {};
		~VertexArray() {};

		virtual void bind() const = 0;
		virtual void unBind() const = 0;

		virtual void addVertexBuffer(Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void setIndexBuffer(Ref<IndexBuffer>& indexBuffer) = 0;

		inline Ref<VertexBuffer> getVertexBuffers() const { return vertexBuffers_; }
		inline Ref<IndexBuffer> getIndexBuffer() const { return indexBuffer_; }

	protected:
		Ref<VertexBuffer> vertexBuffers_;
		Ref<IndexBuffer> indexBuffer_;
		u32 render_id = 0;
	};



#endif 