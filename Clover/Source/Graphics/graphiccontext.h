#ifndef _GRAPHIC_CONTEXT_H_
#define _GRAPHIC_CONTEXT_H_ 1


#include <Clover.h>


#include "buffer.h"
#include "framebuffer.h"
#include "texture.h"
#include "shader.h"
#include "vertexarray.h"
#include "material.h"
#include "render_pass.h"


  class Window;
  class WindowProps;

  //Interface of the Graphic API Context with the window
  class GraphicContext : public Cl_Object
  {
  public:

    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;

    static Ref<GraphicContext> CreateContext(void* window);

	static Ref<VertexBuffer> CreateVertexBuffer(unsigned int count);
	static Ref<VertexBuffer> CreateVertexBuffer(float* vertex, u32 count);

	static Ref<IndexBuffer> CreateIndexBuffer(u32 size);
	static Ref<IndexBuffer> CreateIndexBuffer(u32* index_, u32 size);

	static Ref<RenderPass> CreateRenderPass(const RenderPassSpecification& specs);
	static Ref<FrameBuffer> CreateFrameBuffer(const FramebufferSpecification& specs);

	static Ref<Shader> CreateShader(const CString& name, const CString& fragment_src, const CString& vertex_src);
	static Ref<Shader> CreateShader(const CString& name, const CString& filepath);

	static Ref<Texture2D> CreateTexture2D(const CString& filepath, bool relative = true);
	static Ref<Texture2D> CreateTexture2D(const u32 width, const u32 height, const Texture::TextureProps& properties);
	static Ref<SubTexture2D> CreateSubTexture2D(const Ref<Texture2D> Tex, const glm::vec2& Coords, const glm::vec2& CellSize, const glm::vec2& SpriteSize = {1, 1});

	static Ref<VertexArray> CreateVertexArray();

	static Window* CreateCloverWindow(const WindowProps& windowProps);

	static Ref<Material> CreateMaterial(const Ref<Shader>& shader);



  };


#endif
