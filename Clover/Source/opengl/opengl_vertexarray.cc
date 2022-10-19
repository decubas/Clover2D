#include "Clover.h"

#include <opengl/opengl_vertexarray.h>

OpenGLVertexArray::OpenGLVertexArray()
{
	glGenVertexArrays(1, &render_id);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
	glDeleteVertexArrays(1, &render_id);
}

void OpenGLVertexArray::bind() const
{
	glBindVertexArray(render_id);
	indexBuffer_->bind();
}

void OpenGLVertexArray::unBind() const
{
	glBindVertexArray(0);
}

void OpenGLVertexArray::addVertexBuffer(Ref<VertexBuffer>& vertexBuffer)
{
	glBindVertexArray(render_id);

	vertexBuffer->bind();

	u32 index = 0;
	const auto& layout = vertexBuffer->getLayout();
	for (const VertexBufferElement& element : layout.getElements())
	{
		switch (element.type_)
		{
			case ShaderDataType::Float:  
			case ShaderDataType::Float2: 
			case ShaderDataType::Float3: 
			case ShaderDataType::Float4: 
			case ShaderDataType::Bool:   
			case ShaderDataType::Mat3:   
			case ShaderDataType::Mat4:   
			{
				glEnableVertexAttribArray(index);

				glVertexAttribPointer(index,
					element.GetComponentCount(),
					OpenGLHelper::ShaderDataTypeOpenGL(element.type_),
					element.normalized_ ? GL_TRUE : GL_FALSE,
					layout.stride,
					(const void*)element.offset_);
				index++;
				break;
			}
			case ShaderDataType::UInt:   
			case ShaderDataType::Int:    
			case ShaderDataType::Int2:   
			case ShaderDataType::Int3:   
			case ShaderDataType::Int4:   
			{
				glEnableVertexAttribArray(index);

				glVertexAttribIPointer(index,
					element.GetComponentCount(),
					OpenGLHelper::ShaderDataTypeOpenGL(element.type_),
					layout.stride,
					(const void*)element.offset_);
				index++;
				break;
			}
		}
		
		
	}

	vertexBuffers_ = vertexBuffer;
}

void OpenGLVertexArray::setIndexBuffer(Ref<IndexBuffer>& indexBuffer)
{
	indexBuffer_ = indexBuffer;
}
