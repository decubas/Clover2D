#pragma once

#include "framebuffer.h"

	struct RenderPassSpecification
	{
		Ref<FrameBuffer> targetFramebuffer;
	};

	class RenderPass : public Cl_Object
	{
	public:
		
		RenderPass(const RenderPassSpecification& specs);

		void Begin(bool clear = true);
		void End();

		RenderID GetColorAttachment() { return m_Specifications.targetFramebuffer->getColorAttachment(); }

		RenderPassSpecification& GetSpecification() { return m_Specifications; }

	private:
		RenderPassSpecification m_Specifications;
		EntityID m_HoveredEntity;
	};

