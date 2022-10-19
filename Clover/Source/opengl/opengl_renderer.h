#ifndef _OPENGL_RENDER2D_H
#define _OPENGL_RENDER2D_H

#include "Graphics/renderer.h"


	// OpenGL Api Commands
    class OpenGLRendererApi : public RendererApi
    {
    public:
        virtual void Init() override;

		virtual void setClearColor(const glm::vec4& color = glm::vec4()) override;
		virtual void Clear() override;
		virtual void enable(int setting) override;
		virtual void disable(int setting) override;
		virtual void setCullFace(int cullFace) override;
		virtual void setDrawMode(int mode) override;


		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;

    };

#endif
