
#include "opengl_buffer.h"

    /////////////////////////////////////////////////////////////////////////
    /////VERTEX BUFFER ///////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////


    OpenGLVertexBuffer::OpenGLVertexBuffer(unsigned int count)
    {
        assert(count != 0);


        count_ = count;
        glGenBuffers(1, &render_id);
        glBindBuffer(GL_ARRAY_BUFFER, render_id);
        glBufferData(GL_ARRAY_BUFFER, count, NULL, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertex, unsigned int count)
    {
        assert(count != 0);

        count_ = count;
        glGenBuffers(1, &render_id);
        glBindBuffer(GL_ARRAY_BUFFER, render_id);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), vertex, GL_STATIC_DRAW);
    }



    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        release();
    }



    void OpenGLVertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, render_id);
    }


    void OpenGLVertexBuffer::uploadData(
        const void *data,
        unsigned int size,
        unsigned int offset) {

        bind();
        
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        OpenGLHelper::GLGetError("Upload Data Vertex Buffer");
    }

	void OpenGLVertexBuffer::setLayout(const VertexBufferLayout& layout)
	{
        layout_ = layout;
	}

    VertexBufferLayout& OpenGLVertexBuffer::getLayout()
    {
        return layout_;
    }

    void OpenGLVertexBuffer::release() {
        glDeleteBuffers(1, &render_id);
    }
    /////////////////////////////////////////////////////////////////////////
    /////INDEX BUFFER ///////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////


	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int count)
	{
		assert(count != 0);



		size_ = count;
		glGenBuffers(1, &render_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* index, unsigned int count)
	{
		assert(count != 0);


		count_ = count;
		glGenBuffers(1, &render_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_ * sizeof(unsigned int), index, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
        release();
    }


    void OpenGLIndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_id);

    }


    void OpenGLIndexBuffer::uploadData(
        const void* data,
        unsigned int size,
        unsigned int offset) {

        bind();


        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
        OpenGLHelper::GLGetError("Upload Data Index Buffer");
    }

    void OpenGLIndexBuffer::release() {
        glDeleteBuffers(1, &render_id);
    }
