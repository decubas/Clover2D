#include "scene_renderer2D.h"
#include "Graphics/graphiccontext.h"
#include "Graphics/renderer2D.h"
#include "Graphics/render_command.h"

#include <glad/glad.h>

	SceneRenderer2D::SceneRenderer2D(const Ref<Scene>& scene)
		: m_Scene(scene){}

	void SceneRenderer2D::Init()
	{
		FramebufferSpecification specs;
		RenderPassSpecification rpSpecs;
		rpSpecs.targetFramebuffer = GraphicContext::CreateFrameBuffer(specs);
		m_ColorPass = GraphicContext::CreateRenderPass(rpSpecs);
	}

	void SceneRenderer2D::SetScene(const Ref<Scene>& scene)
	{
		m_Scene = scene;
	}

	// Editor
	void SceneRenderer2D::Render(EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		Renderer::BeginScene(camera);

		m_ColorPass->Begin(true);
		FillRenderList();
		Renderer::Submit([=]() {
			m_HoveredEntityID = Pixel((s32)m_MousePosition.x, (s32)m_MousePosition.y);
		});
		m_ColorPass->End();
		
		Renderer::EndScene();
		Renderer2D::EndScene();

	}

	// RUNTIME
	void SceneRenderer2D::Render()
	{
		if (!m_Scene->IsCameraValid()) return;


		Renderer2D::BeginScene(m_Scene->GetCurrentCamera(), m_Scene->GetCurrentCameraTransform());


		m_ColorPass->Begin(true);
		FillRenderList();
		m_ColorPass->End();


		Renderer2D::EndScene();
	}


	void SceneRenderer2D::Resize(u32 width, u32 height)
	{
		m_Scene->OnViewportResize(width, height);
		m_ColorPass->GetSpecification().targetFramebuffer->resize(width, height);
	}


	void SceneRenderer2D::FillRenderList()
	{
		/*if (m_Scene->m_ecs.existsComponent<SpriteRenderComponent>())
		{
			m_Scene->m_ecs.forEach<SpriteRenderComponent>([&](auto* sprite)
				{
					glm::mat4 transform = Entity(m_Scene, sprite->getOwnerID()).getComponent<TransformComponent>().getTransformMatrix();

					if (sprite->texture)
					{
						Renderer2D::DrawQuad(transform, sprite->texture, sprite->getOwnerID());
					}
					else
					{
						Renderer2D::DrawQuad(transform, sprite->Color, sprite->getOwnerID());
					}
				});
			Renderer2D::Flush();
		}

		if (m_Scene->m_ecs.existsComponent<BoxCollider2DComponent>())
		{
			m_Scene->m_ecs.forEach<BoxCollider2DComponent>([&](auto* sprite)
				{
					glm::mat4 transform = Entity(m_Scene, sprite->getOwnerID()).getComponent<TransformComponent>().getTransformMatrix();
					Renderer2D::DrawQuad(transform, { 0, 255, 0, 100}, sprite->getOwnerID());
				});
			Renderer2D::Flush();
		}*/
	}

	int SceneRenderer2D::Pixel(s32 x, s32 y)
	{

		glReadBuffer(GL_COLOR_ATTACHMENT1);
		int pixel;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel);
		return pixel;
	}


	glm::vec4 SceneRenderer2D::PixelColor(s32 x, s32 y)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		float pixel[4];
		glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, (void*)pixel);
		return { pixel[0], pixel[1], pixel[2], pixel[0] };
	}
