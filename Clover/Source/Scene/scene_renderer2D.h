#pragma once

#include "Graphics/render_pass.h"
#include "scene.h"


class SceneRenderer2D
{
public:
	SceneRenderer2D() = default;
	SceneRenderer2D(const Ref<Scene>& scene);

	void Init();

	void SetScene(const Ref<Scene>& scene);
	Ref<Scene>& GetScene() { return m_Scene; };

	RenderID GetColorAttachment() { return m_ColorPass->GetColorAttachment(); }

	void Render(EditorCamera& camera);
	void Render();

	int GetHoveredEntityID() { return m_HoveredEntityID; };
	void Resize(u32 width, u32 height);

	
	glm::vec2 m_MousePosition;

private:
	void FillRenderList();

	s32 Pixel(s32 x, s32 y);
	glm::vec4 PixelColor(s32 x, s32 y);

private:
	Ref<Scene> m_Scene;
	s32 m_HoveredEntityID = -1;
	// Render Passes
	Ref<RenderPass> m_ColorPass;

};

