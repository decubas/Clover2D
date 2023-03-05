
#include "SceneViewportPanel.h"
#include <Editor/Asset.h>
#include <ImGuizmo.h>
#include "CloverEngine.h"


	SceneViewportPanel::SceneViewportPanel(const Ref<Scene>& context)
	{
		SetContext(context);
		CloverEngine* Engine = CloverEngine::GetEngine();
		Engine->m_Window->OnKeyPressed.AddFunction(this, &SceneViewportPanel::OnKeyPressed);
	}

	void SceneViewportPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectedObject = -1;
	}

	void SceneViewportPanel::SetSelectedObject(UniqueID id)
	{
		if (id >= 0)	m_SelectedObject = id;
		else m_SelectedObject = -1;
	}

	void SceneViewportPanel::OnImGuiRender(RenderID textureID, EditorCamera& editorCamera)
	{
		RenderScene(textureID);

		// Gizmo's
		DrawGuizmos(editorCamera);


		ImGui::End();
		ImGui::PopStyleVar();

	}


	void SceneViewportPanel::OnImGuiRenderRuntime(RenderID textureID)
	{
		RenderScene(textureID);

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void SceneViewportPanel::OnKeyPressed(s32 key, s32 count)
	{
		if (IsViewportFocus())
		{
			switch (key)
			{
				// Gizmo's Shortcuts
			case Key::Q:
				m_GizmoType = -1;
				break;
			case Key::W:
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case Key::E:
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case Key::R:
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
		}
	}

	void SceneViewportPanel::DrawGuizmos(EditorCamera& editorCamera)
	{
		if (!IsSelectedEntityValid()) return;

		/*Entity selectedEntity(m_Context, m_SelectedObject);
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Camera

			// Runtime camera from entity
			//ECS::IEntity& cameraEntity = m_Scene->GetPrimaryCameraEntity();
			//const auto& camera = cameraEntity.getComponent<CameraComponent>().Camera;
			//const glm::mat4& cameraProjection = camera.GetProjection();
			//glm::mat4 cameraView = glm::inverse(cameraEntity.getComponent<TransformComponent>().getTransformMatrix());

			// Editor camera
			const glm::mat4& cameraProjection = editorCamera.GetProjection();
			glm::mat4 cameraView = editorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.getComponent<TransformComponent>();
			glm::mat4 transform = tc.getTransformMatrix();

			// Snapping
			bool snap = Input::isKeyPressed(Key::LeftControl);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				MathUtils::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.m_Rotation;
				tc.m_Position = translation;
				tc.m_Rotation += deltaRotation;
				tc.m_Scale = scale;
			}
		}*/
	}


	void SceneViewportPanel::RenderScene(RenderID textureID)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGuiWindowClass window_class;
		window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
		ImGui::SetNextWindowClass(&window_class);
		ImGui::Begin("Viewport");

		auto viewportOffset = ImGui::GetCursorPos();

		m_ViewportFocus = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsAnyWindowHovered();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != glm::vec2{ viewportPanelSize.x, viewportPanelSize.y })
		{
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
		m_ViewportBounds[0] = { minBound.x, minBound.y };
		m_ViewportBounds[1] = { maxBound.x, maxBound.y };
	}

	glm::vec2 SceneViewportPanel::GetViewportMousePos()
	{
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		auto viewportWidth = m_ViewportBounds[1].x - m_ViewportBounds[0].x;
		auto viewportHeight = m_ViewportBounds[1].y - m_ViewportBounds[0].y;
		my = viewportHeight - my;
		return { (int)mx, (int)my };
	}

	glm::vec2 SceneViewportPanel::GetViewportSize()
	{
		auto viewportWidth = m_ViewportBounds[1].x - m_ViewportBounds[0].x;
		auto viewportHeight = m_ViewportBounds[1].y - m_ViewportBounds[0].y;
		return { viewportWidth, viewportHeight };
	}
