#include "Clover.h"

#include "opengl_renderer.h"
#include "opengl.h"

 
	void OpenGLRendererApi::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glEnable(GL_DEBUG_OUTPUT); 
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(MessageCallback, nullptr);
		
		//glDisable(GL_CULL_FACE);
	    //glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
	}

/*
	Renderer* Renderer::Create()
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::API::none: assert(false);
		case RendererApi::API::OpenGL: return new OpenGLRendererApi();
		}
	}*/

	void OpenGLRendererApi::setClearColor(const glm::vec4& color /*= CColor()*/)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererApi::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearDepth(1);
	}

	void OpenGLRendererApi::enable(int setting)
	{
		GLenum result = GL_INVALID_ENUM;
		switch (setting)
		{
// 		case CF_BLEND: result = GL_BLEND;
// 			break;
// 		case CF_DEPTH: result = GL_DEPTH;
// 			break;
// 		default:
// 			break;
		}
		glEnable(result);
	}

	void OpenGLRendererApi::disable(int setting)
	{
		glDisable(setting);
	}

	void OpenGLRendererApi::setCullFace(int cullFace)
	{
		glCullFace(cullFace);
	}

	void OpenGLRendererApi::setDrawMode(int mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, mode); 
	}

	void OpenGLRendererApi::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount != 0 ? indexCount : vertexArray->getIndexBuffer()->count();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
