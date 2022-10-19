#include "Clover.h"

#include "Graphics/render_pass.h"
#include "Graphics/renderer2D.h"
#include "Graphics/render_command.h"

 
	RenderPass::RenderPass(const RenderPassSpecification& specs)
		: m_Specifications(specs)
	{}

	void RenderPass::Begin(bool clear)
	{
		m_Specifications.targetFramebuffer->bind();
		if (clear)
		{
			Renderer::Submit([=]() {

				RendererCommand::clearColor(m_Specifications.targetFramebuffer->GetSpecifications().ClearColor);
				RendererCommand::Clear();

			});
		}
		m_Specifications.targetFramebuffer->bind();
	}

	void RenderPass::End()
	{
		m_Specifications.targetFramebuffer->unBind();
	}
