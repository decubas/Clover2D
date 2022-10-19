#ifndef _OPENGL_FRAME_BUFFER_
#define _OPENGL_FRAME_BUFFER_ 1

#include "Graphics/framebuffer.h"


	class  OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FramebufferSpecification& specs);


		virtual void invalidate() override;
		virtual void resize(u32 width, u32 height) override;
		virtual void bind() const override;
		virtual void unBind() const override;


		virtual ~OpenGLFrameBuffer();



	protected:
	};
#endif
