#ifndef _OPENGL_VERTEXARRAY_H__
#define _OPENGL_VERTEXARRAY_H__

#include "opengl.h"
#include "opengl_buffer.h"

#include "Graphics/vertexarray.h"
#include "Graphics/renderer.h"


    class OpenGLVertexArray : public VertexArray {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        virtual void bind() const override;
        virtual void unBind() const override;

        virtual void addVertexBuffer(Ref<VertexBuffer>& vertexBuffer) override;
        virtual void setIndexBuffer(Ref<IndexBuffer>& vertexBuffer) override;

    };

#endif