#ifndef _FRAME_BUFFER_
#define _FRAME_BUFFER_ 1

#include <Clover.h>

#include "texture.h"


	// SPECIFICATIONS
	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(const std::initializer_list<Texture::Format>&
			attachments) : Attachments(attachments)
		{}

		std::vector<Texture::Format> Attachments;
	};
	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(const Texture::Format& format,
			const Texture::Filter& filter = Texture::Filter::LINEAR,
			const Texture::Wrap& wrap = Texture::Wrap::CLAMP_TO_EDGE) : Format(format), Filter(filter), Wrap(wrap)
		{}

		Texture::Wrap Wrap = Texture::Wrap::CLAMP_TO_EDGE;
		Texture::Filter Filter = Texture::Filter::LINEAR;
		Texture::Format Format = Texture::Format::RGBA16F;
	};

	struct FramebufferSpecification
	{
		FramebufferSpecification() = default;
		FramebufferSpecification(u32 width, u32 height)
			: Width(width), Height(height)
		{}
		u32 Width = 1280, Height = 720;
		s32 Samples = 1;
		glm::vec4 ClearColor = { 0.9f, 0.3f, 0.3f, 1.0f };
		FramebufferAttachmentSpecification Attachments;
	};


	class FrameBuffer : public Cl_Object
	{
	public:


		inline FramebufferSpecification& GetSpecifications() { return m_Specs; }
		

		virtual void invalidate() = 0;
		virtual void resize(u32 width, u32 height) = 0;
		RenderID getColorAttachment() { return m_ColorAttachment->id(); }
		Ref<Texture2D> getColorTexture() {return m_ColorAttachment;}


		virtual void bind() const = 0;
		virtual void unBind() const = 0;


		virtual ~FrameBuffer() = default;

	protected:
		RenderID render_id = 0;
		//RenderID colorAttachment_ = 0;
		RenderID depthAttachment_ = 0;
		Ref<Texture2D> m_ColorAttachment;
		FramebufferSpecification m_Specs;
	};

#endif
