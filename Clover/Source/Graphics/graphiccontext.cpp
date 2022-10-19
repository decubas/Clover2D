#include "Clover.h"

#include <Graphics/graphiccontext.h>
#include <Graphics/renderer.h>

//OpenGL includes
#include <Window.h>
#include <opengl/opengl_context.h>
#include <opengl/opengl_buffer.h>
#include <opengl/opengl_framebuffer.h>
#include <opengl/opengl_shader.h>
#include <opengl/opengl_texture.h>
#include <opengl/opengl_vertexarray.h>

 
	Ref<GraphicContext> GraphicContext::CreateContext(void* window)
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::API::none: assert(false);
		case RendererApi::API::OpenGL: return CreateRef<OpenGLContext>((GLFWwindow*)window);
		}
		return nullptr;
	}

	Ref<VertexBuffer> GraphicContext::CreateVertexBuffer(unsigned int size_)
	{

		switch (RendererApi::GetApi())
		{
		case RendererApi::API::none: assert(false);
		case RendererApi::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(size_);
		}
		return nullptr;
	}

	Ref<VertexBuffer> GraphicContext::CreateVertexBuffer(float* vertex_, u32 size_)
	{

		switch (RendererApi::GetApi())
		{
		case RendererApi::API::none: assert(false);
		case RendererApi::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertex_, size_);
		}
		return nullptr;
	}


	Ref<IndexBuffer> GraphicContext::CreateIndexBuffer(u32 size_)
	{

		switch (RendererApi::GetApi())
		{
		case RendererApi::API::none: assert(false);
		case RendererApi::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(size_);
		}
		return nullptr;
	}

	Ref<IndexBuffer> GraphicContext::CreateIndexBuffer(u32* index_, u32 size_)
	{

		switch (RendererApi::GetApi())
		{
		case RendererApi::API::none: assert(false);
		case RendererApi::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(index_, size_);
		}
		return nullptr;
	}


	Ref<RenderPass> GraphicContext::CreateRenderPass(const RenderPassSpecification& specs)
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::API::none: assert(false);
		case RendererApi::API::OpenGL: return  CreateRef<RenderPass>(specs);
		}
		return nullptr;
	}

	Ref<FrameBuffer> GraphicContext::CreateFrameBuffer(const FramebufferSpecification& specs)
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::API::none: assert(false);
		case RendererApi::API::OpenGL: return  CreateRef<OpenGLFrameBuffer>(specs);
		}
		return nullptr;
	}

	Ref<Shader> GraphicContext::CreateShader(const CString& name, const CString& fragment_src, const CString& vertex_src)
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::API::none: assert(false);
		case RendererApi::API::OpenGL: return  CreateRef<OpenGLShader>(name, fragment_src, vertex_src);
		}
		return nullptr;
	}

	Ref<Shader> GraphicContext::CreateShader(const CString& name, const CString& filepath)
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::API::none: assert(false);
		case RendererApi::API::OpenGL: return  CreateRef<OpenGLShader>(name, filepath);
		}
		return nullptr;
	}

	Ref<Texture2D> GraphicContext::CreateTexture2D(const CString& filepath, bool relative)
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::API::none: assert(false);
		case RendererApi::API::OpenGL: return CreateRef<OpenGLTexture2D>(filepath, relative);
		}
		return nullptr;
	}

	Ref<Texture2D> GraphicContext::CreateTexture2D(const u32 width, const u32 height, const Texture::TextureProps& properties)
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::API::none: assert(false);
		case RendererApi::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height, properties);
		}
		return nullptr;
	}

	Ref<SubTexture2D> GraphicContext::CreateSubTexture2D(const Ref<Texture2D> Tex, const glm::vec2& Coords, const glm::vec2& CellSize, const glm::vec2& SpriteSize)
	{
		return CreateRef<SubTexture2D>(Tex, Coords, CellSize, SpriteSize);
	}

	Ref<VertexArray> GraphicContext::CreateVertexArray()
	{

		switch (RendererApi::GetApi())
		{
		case RendererApi::API::none: Cl_Assert(false, "No API");
		case RendererApi::API::OpenGL: return CreateRef<OpenGLVertexArray>();
		}
		return nullptr;
	}

	Window* GraphicContext::CreateCloverWindow(const WindowProps& windowProps)
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::API::none: Cl_Assert(false, "No API");
		case RendererApi::API::OpenGL: return (Window*)new WindowsWindow(windowProps);
		}
		return nullptr;
	}

	Ref<Material> GraphicContext::CreateMaterial(const Ref<Shader>& shader)
	{
		return CreateRef<Material>(shader);
	}
