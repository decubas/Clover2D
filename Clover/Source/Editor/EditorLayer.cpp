
#include "EditorLayer.h"
#include "CloverEngine.h"

#include "Asset.h"

#include <Scene/scene_serializer.h>
#include <Utils/PlatformUtils.h>
#include <ImGuizmo.h>
#include <Graphics/camera.h>
#include <Scene/scene.h>
#include "Graphics/renderer2D.h"


	void EditorLayer::OnAttach()
	{
		m_EditorCamera = EditorCamera();
		m_Scene = CreateRef<Scene>();	
		SetScene();
		m_SceneRenderer.Init();


/*
 		CHAR currentDir[256] = { 0 };
 		GetCurrentDirectoryA(256, currentDir);
 		std::string currDir = currentDir;
 		m_FileSystem.Initialize(currDir + "/assets");*/

		CloverEngine* Engine = CloverEngine::GetEngine();
		Engine->m_Window->OnKeyPressed.AddFunction(this, &EditorLayer::OnKeyPressed);
		Engine->m_Window->OnMouseButtonPressed.AddFunction(this, &EditorLayer::OnMouseButtonPressed);
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(float delta)
	{

		if (m_ViewportPanel.IsViewportFocus() && !ImGuizmo::IsUsing())
		{
			//update camera controller
			m_EditorCamera.OnUpdate(delta);
		}
		if (m_SceneState == SceneState::Editor || m_SceneState == SceneState::Pause)
		{
			m_Scene->OnUpdateEditor(delta);
		}
		else
		{
			m_Scene->OnUpdateRuntime(delta);
		}	

	}

	void EditorLayer::OnRender()
	{

		////////////////// PREPARE RENDER
		if (m_ViewportPanel.GetViewportPanelSize() != glm::vec2{ m_Scene->GetViewportWidth(), m_Scene->GetViewportHeight() })
		{
			Resize();
		}


		Renderer2D::ResetStats();

		if (m_SceneState == SceneState::Editor || m_SceneState == SceneState::Pause)
		{
			auto mouse = m_ViewportPanel.GetViewportMousePos();
			m_SceneRenderer.m_MousePosition = mouse;
			if (!loadingScene) m_SceneRenderer.Render(m_EditorCamera);

			auto viewportSize = m_ViewportPanel.GetViewportSize();

			if (mouse.x >= 0 && mouse.y >= 0 && mouse.x < viewportSize.x && mouse.y < viewportSize.y)
			{
				m_HoveredEntityID = m_SceneRenderer.GetHoveredEntityID();
			}
			else
			{
				m_HoveredEntityID = -2;
			}
		}
		else
		{
			if (!loadingScene) m_SceneRenderer.Render();
		}


		loadingScene = false;

		renderID = m_SceneRenderer.GetColorAttachment();



	}

	void EditorLayer::OnImGuiRender()
	{

		bool dockingEnable = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (dockingEnable)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockingEnable, window_flags);
		ImGui::PopStyleVar();

		ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 300.0f;
		ImGuiID dockspace_id = ImGui::GetID("CoffeeDockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		style.WindowMinSize.x = 32.0f;

		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Shaders"))
				{
					if (ImGui::MenuItem("Compile", "Ctrl+R"))
					{
						Cl_Info("Compiling Shaders..");
						Asset::Shaders().recompile();
					}

					ImGui::EndMenu();
				}


				if (ImGui::BeginMenu("Scene"))
				{
					if (ImGui::MenuItem("New...", "Ctrl+N"))
					{
						NewScene();
					}

					if (ImGui::MenuItem("Open...", "Ctrl+O"))
					{
						OpenScene();
					}
					if (ImGui::MenuItem("Save...", "Ctrl+S"))
					{
						SaveScene();
					}
					if (ImGui::MenuItem("SaveAs...", "Ctrl+Shift+S"))
					{
						SaveSceneAs();
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}
		}

		{
			ImGui::Begin("Scene Settings");
			m_Scene->OnImGuiRender();
			ImGui::End();
		}
		
		{
			ImGui::Begin("Renderer Settings");

			auto stats = Renderer2D::GetStats();

			ImGui::Text("General Renderer Stats:");
			ImGui::Text("Total Draw Calls: %d", stats.DrawCalls);

			ImGui::Separator();

			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			ImGui::End();
		}

		m_ViewportPanel.OnImGuiRender(renderID, m_EditorCamera);
		m_HierarchyPanel.OnImGuiRender();
		/*ImGui::Begin("Project");
		m_FileSystem.ImGuiShow();
		ImGui::End();
*/


		ImGui::End();


	}
	
	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;
		m_ReloadScene = CreateRef<Scene>();
		m_ReloadScene->Copy(m_Scene);

		if (m_ReloadScriptOnPlay)
			//ScriptEngine::ReloadAssembly("assets/scripts/ExampleApp.dll");

		//ScriptEngine::SetSceneContext(m_Scene);
		m_Scene->OnScenePlay();
	}

	void EditorLayer::OnScenePause()
	{
		m_SceneState = SceneState::Pause;
	}

	void EditorLayer::OnSceneStop()
	{

		m_Scene->OnSceneStop();

		m_SceneState = SceneState::Editor;
		ReloadScene();

	}

	void EditorLayer::OnKeyPressed(s32 key, s32 Count)
	{
		if (Count > 0) return;

		if (!ImGuizmo::IsUsing())
		{
			bool control = Input::isKeyPressed(Key::RightControl) || Input::isKeyPressed(Key::LeftControl);
			bool shift = Input::isKeyPressed(Key::RightShift) || Input::isKeyPressed(Key::LeftShift);
			if (control)
			{
				switch (key)
				{
				case Key::O: OpenScene(); break;
				case Key::S:
				{
					if (shift)SaveSceneAs();
					else SaveScene();
				}
				break;
				case Key::N: NewScene(); break;

				case Key::R: Asset::Shaders().recompile(); break;
				}
			}

			
		}
	}

	void EditorLayer::OnMouseButtonPressed(s32 key)
	{
		if (m_HoveredEntityID == -2) return;
		
		bool alt = Input::isKeyPressed(Key::RightAlt) || Input::isKeyPressed(Key::LeftAlt);
		if (key == Mouse::ButtonLeft && !ImGuizmo::IsUsing() && !alt)
		{
			m_HierarchyPanel.SetSelectedObject(m_HoveredEntityID);
			m_ViewportPanel.SetSelectedObject(m_HoveredEntityID);
		}
	}

	void EditorLayer::Resize()
	{
		m_EditorCamera.SetViewportSize(m_ViewportPanel.GetViewportPanelSize().x, m_ViewportPanel.GetViewportPanelSize().y);
		m_SceneRenderer.Resize((u32)m_ViewportPanel.GetViewportPanelSize().x, (u32)m_ViewportPanel.GetViewportPanelSize().y);
	}

	void EditorLayer::NewScene()
	{
		m_Scene = CreateRef<Scene>();
		Resize();
		SetScene();
	}

	void EditorLayer::OpenScene()
	{
		m_SceneFilepath = "";
		std::optional<std::string> filepath = FileDialogs::OpenFile("Coffee Scene (*.coffee)\0*.coffee\0");
		if (filepath)
		{
			loadingScene = true;
			m_Scene = CreateRef<Scene>();
			SceneSerializer serializer(m_Scene);
			serializer.Deserialize(*filepath);

			SetScene();

			Resize();
			m_SceneFilepath = *filepath;
		}
	}

	void EditorLayer::ReloadScene()
	{
		if (m_SceneFilepath != "")
		{
			loadingScene = true;
			m_Scene = CreateRef<Scene>();
			m_Scene->Copy(m_ReloadScene);
			//SceneSerializer serializer(m_Scene);
			//serializer.Deserialize(m_SceneFilepath);

			SetScene();
			Resize();
		}
	}

	void EditorLayer::SaveScene()
	{
		if (m_SceneFilepath != "")
		{
			std::string tempPath = m_SceneFilepath.replace(m_SceneFilepath.find_last_of("\\")+1, m_SceneFilepath.find_last_of("."), m_Scene->GetName().c_str());
			m_SceneFilepath = tempPath + ".coffee";
			SceneSerializer serializer(m_Scene);
			serializer.Serialize(m_SceneFilepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::optional<std::string> filepath = FileDialogs::SaveFile("Coffee Scene (*.coffee)\0*.coffee\0");
		if (filepath)
		{
			SceneSerializer serializer(m_Scene);
			serializer.Serialize(*filepath);
		}
	}

	void EditorLayer::SetScene()
	{
		m_SceneRenderer.SetScene(m_Scene);
		m_HierarchyPanel.SetContext(m_Scene);
		m_ViewportPanel.SetContext(m_Scene);
		//ScriptEngine::SetSceneContext(m_Scene);
	}

