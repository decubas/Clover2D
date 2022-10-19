#ifndef OPENGL_BUFFER_H_
#define OPENGL_BUFFER_H_ 1

#include "opengl.h"

#include "Graphics/buffer.h"
#include "Graphics/renderer.h"

 
  class OpenGLVertexBuffer : public  VertexBuffer {
  public:
      OpenGLVertexBuffer() {};
    OpenGLVertexBuffer(unsigned int count);
    OpenGLVertexBuffer(float* vertex, unsigned int count);


    void bind() const override;
    void uploadData(
        const void *data, 
        unsigned int size, 
        unsigned int offset = 0) override;
    void release() override;

    void setLayout(const VertexBufferLayout& layout) override;
    VertexBufferLayout& getLayout() override;

     ~OpenGLVertexBuffer();


  };

  class OpenGLIndexBuffer : public  IndexBuffer {
  public:
      OpenGLIndexBuffer() {};

      OpenGLIndexBuffer(unsigned int size);
      OpenGLIndexBuffer(unsigned int* index, unsigned int size);

      void bind() const override;
      void uploadData(
          const void* data,
          unsigned int size,
          unsigned int offset = 0) override;
      void release() override;

      ~OpenGLIndexBuffer();
  };


#endif
