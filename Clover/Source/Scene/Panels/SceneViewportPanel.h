#pragma once

#include <Clover.h>
#include <Scene/scene.h>

	class SceneViewportPanel
	{
	public:
		SceneViewportPanel()
		{
			m_ViewportBounds[0] = { 0, 0 };
			m_ViewportBounds[1] = { 1, 1 };
		}
		SceneViewportPanel(const Ref<Scene>& context);
		~SceneViewportPanel() {};

		void SetContext(const Ref<Scene>& context);

		void SetSelectedObject(UniqueID id);
		UniqueID GetSelectedEntity() { return m_SelectedObject; }
		void OnImGuiRender(RenderID textureID, EditorCamera& editorCamera);
		void OnImGuiRenderRuntime(RenderID textureID);
		bool IsSelectedEntityValid() { return m_SelectedObject >= 0; }

		bool IsViewportFocus() { return m_ViewportFocus; }
		bool IsViewportHovered() { return m_ViewportHovered; }

		glm::vec2& GetViewportBounds(s32 index) { return m_ViewportBounds[index]; }
		glm::vec2& GetViewportPanelSize() { return m_ViewportSize; };
		glm::vec2 GetViewportSize();

		glm::vec2 GetViewportMousePos();
		void OnKeyPressed(s32 key, s32 count);
	private:
		void DrawGuizmos(EditorCamera& editorCamera);
		void RenderScene(RenderID textureID);

	protected:
		bool m_ViewportFocus = false;
		bool m_ViewportHovered = false;

		glm::vec2 m_ViewportSize = { 16.0f, 16.0f };
		glm::vec2 m_ViewportBounds[2];
		
		int m_GizmoType = -1;

		Ref<Scene> m_Context;
		UniqueID m_SelectedObject = -1;
	};


