#include "Clover.h"

#include "opengl_framebuffer.h"
#include "opengl.h"

#include <Graphics/graphiccontext.h>
#include <Graphics/renderer.h>

	static const uint32_t s_MaxFramebufferSize = 8192;



	OpenGLFrameBuffer::OpenGLFrameBuffer(const FramebufferSpecification& specs) 
	{
		m_Specs = specs;
		resize(m_Specs.Width, m_Specs.Height);
	}

	void OpenGLFrameBuffer::invalidate()
	{
		Ref<OpenGLFrameBuffer> instance;
		instance.reset((OpenGLFrameBuffer*)this);
		
		Renderer::Submit([instance]() {
			if (instance->render_id)
			{
				glDeleteFramebuffers(1, &instance->render_id);
				instance->m_ColorAttachment->Delete();
				glDeleteTextures(1, &instance->depthAttachment_);
			}

			glCreateFramebuffers(1, (GLuint*)&instance->render_id);
			glBindFramebuffer(GL_FRAMEBUFFER, instance->render_id);
			});

		Texture2D::TextureProps props;
		props.format = Texture::Format::RGBA8;
		props.wrap = Texture::Wrap::CLAMP_TO_EDGE;
		props.filter = Texture::Filter::LINEAR;

		m_ColorAttachment = GraphicContext::CreateTexture2D(m_Specs.Width, m_Specs.Height, props);

		Renderer::Submit([instance]() {

			glBindTexture(GL_TEXTURE_2D, instance->m_ColorAttachment->id());
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, instance->m_ColorAttachment->id(), 0);
			OpenGLHelper::GLGetError("Color Framebuffer");

			//////////////

			GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
			glDrawBuffers(1, drawBuffers);


			glCreateTextures(GL_TEXTURE_2D, 1, (GLuint*)&instance->depthAttachment_);
			glBindTexture(GL_TEXTURE_2D, instance->depthAttachment_);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, instance->m_Specs.Width, instance->m_Specs.Height);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, instance->depthAttachment_, 0);
			OpenGLHelper::GLGetError("Depth Framebuffer");


			Cl_Assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer not completed");
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		});
	}

	void OpenGLFrameBuffer::bind() const
	{
		Ref<OpenGLFrameBuffer> instance;
		instance.reset((OpenGLFrameBuffer*)this);

		Renderer::Submit([instance]() {			
			Cl_Assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer not completed");
			glBindFramebuffer(GL_FRAMEBUFFER, instance->render_id);
			glViewport(0, 0, instance->m_Specs.Width, instance->m_Specs.Height);
			OpenGLHelper::GLGetError("Bind Framebuffer");
		});
	}

	void OpenGLFrameBuffer::unBind() const
	{
		Renderer::Submit([]() {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			Cl_Assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer not completed");
			OpenGLHelper::GLGetError("Unbind Framebuffer");
		});
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		Ref<OpenGLFrameBuffer> instance;
		instance.reset((OpenGLFrameBuffer*)this);
		Renderer::Submit([instance]() {
			glDeleteFramebuffers(1, &instance->render_id);
			//glDeleteTextures(1, &instance->colorAttachment_);
			glDeleteTextures(1, &instance->depthAttachment_);
		});
	}

	void OpenGLFrameBuffer::resize(u32 width, u32 height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			Cl_Warning("Attempted to rezize framebuffer to %d, %d", width, height);
			return;
		}
		m_Specs.Width = width;
		m_Specs.Height = height;

		invalidate();		
	}
