#include <CloverEngine.h>

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include <glfw/glfw3.h>
#include "Graphics/UI.h"


bool CloverEngine::Construct(const WindowProps& Properties, bool UseImGui)
{
	Random::Init();

	m_ImGuiEnabled = UseImGui;
	m_ImGuiConstructed = UseImGui;

	m_Window.reset(GraphicContext::CreateCloverWindow(Properties));
	m_Clock.reset();

// 	m_UICamera.SetViewportSize(Properties.width_, Properties.height_);
// 	m_UICamera.SetOrthographic(100, -1, 1);
// 	
// 	m_EditorCamera.SetViewportSize(Properties.width_, Properties.height_);
// 	m_EditorCamera.SetOrthographic(100, -1, 1);

	m_MainRenderLayer = Renderer2D::CreateLayer();
	Renderer::Init();
#if USE_2D_RENDERER
	Renderer2D::Init(m_Window->width(), m_Window->height());
#endif
	//UI::Init(m_Window, &m_UICamera);

	//OnUserConstruct();

	m_Window->OnWindowClose.AddFunction(this, &CloverEngine::Shutdown);

	m_Running = true;

	if (m_ImGuiConstructed)
		ImGuiConstruct();

	Renderer2D::UseLayer(m_MainRenderLayer);

	return m_Window.get() != nullptr;
}

void CloverEngine::Run()
{
	while (m_Running)
	{
		m_DeltaTime = m_Clock.get();

		m_Tick.Broadcast(m_DeltaTime);
		m_TimerManager.UpdateTimers(m_DeltaTime);

		for (Ref<Layer> layer : m_LayerStack)
		{
			layer->OnUpdate(m_DeltaTime);
		}

		
#if USE_2D_RENDERER
		Renderer2D::Clear();
#endif
		/*if (m_SceneViewport.GetViewportSize().x != m_MainScene->GetViewportWidth() && m_SceneViewport.GetViewportSize().y != m_MainScene->GetViewportHeight())
		{
			m_MainScene->OnViewportResize((u32)m_SceneViewport.GetViewportSize().x, (u32)m_SceneViewport.GetViewportSize().y);
		}*/
		/*
 		Renderer2D::UseLayer(m_MainScene->GetRenderLayer());
 		Renderer2D::BeginScene(m_MainScene->GetCurrentCamera(), glm::mat4(1.0f));*/
		for (Ref<Layer> layer : m_LayerStack)
		{
			layer->OnRender();
		}
// 		Renderer2D::EndScene();
		

		Renderer::WaitAndRender();
		Renderer::Swap();
		


		if(m_ImGuiEnabled)
		{
			ImGuiNewFrame();
			for (Ref<Layer> layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			ImGuiEndFrame();
		}


#if USE_2D_RENDERER
		Renderer2D::ResetStats();
#endif
		m_Window->OnUpdate();
	}

	OnUserDestroy();

#if USE_2D_RENDERER
	Renderer2D::Shutdown();
#endif
	OnShutdown();
}

void CloverEngine::Shutdown()
{
	m_Running = false;
}


glm::vec2 CloverEngine::GetMouseScreenPosition()
{
	return { 0, 0 };//m_UICamera.ScreenToWorld({ Input::mouseX(), Input::mouseY() });
}

void CloverEngine::ImGuiConstruct()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Docking
	ImGui::StyleColorsDark();
	ImGuiSetDarkTheme();

	io.Fonts->AddFontFromFileTTF(ASSETS_PATH("fonts/opensans/OpenSans-Bold.ttf").c_str(), 18.0f);
	io.FontDefault = io.Fonts->AddFontFromFileTTF(ASSETS_PATH("fonts/opensans/OpenSans-Regular.ttf").c_str(), 18.0f);

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}


	GLFWwindow* window = static_cast<GLFWwindow*>(m_Window->GetNativeWindow());

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void CloverEngine::ImGuiSetDarkTheme()
{
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}

void CloverEngine::ImGuiBeginDockspace()
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
}

void CloverEngine::ImGuiEndDockspace()
{
	ImGui::End();
}

void CloverEngine::ImGuiShowDrawStats()
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


void CloverEngine::ImGuiNewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void CloverEngine::ImGuiEndFrame()
{
	ImGuiIO& io = ImGui::GetIO();

	io.DisplaySize = ImVec2((float)m_Window->width(), (float)m_Window->height());

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void CloverEngine::OnShutdown()
{
	if (m_ImGuiConstructed)
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	else
	{
		m_Window->shutdown();
	}
	Cl_Info("Engine Shutdown");
}
