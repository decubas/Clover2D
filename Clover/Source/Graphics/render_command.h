#ifndef _BASE_RENDER_COMMAND_H
#define _BASE_RENDER_COMMAND_H

#include <Clover.h>
#include "renderer.h"

#define CloverCommand RendererCommand

	// Wrapper of the Api with static functions
	class RendererCommand
	{
	public:

		inline static void Init()
		{
			rendererApi_->Init();
		}

		inline static void clearColor(const glm::vec4& color = glm::vec4())
		{
			rendererApi_->setClearColor(color);
		}

		inline static void Clear()
		{
			rendererApi_->Clear();
		}

		inline static void setCullFace(int cullFace)
		{
			rendererApi_->setCullFace(cullFace);
		}
		
		inline static void enable(int setting)
		{
			rendererApi_->enable(setting);
		}

		inline static void disable(int setting)
		{
			rendererApi_->disable(setting);
		}

		inline static void setDrawMode(int mode)
		{
			rendererApi_->setDrawMode(mode);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			vertexArray->bind();
			rendererApi_->DrawIndexed(vertexArray, indexCount);
			vertexArray->unBind();
		}

		RendererCommand() {};
		virtual ~RendererCommand() {}


	private:
		static RendererApi* rendererApi_;
	};
#endif
