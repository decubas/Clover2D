#ifndef _CLOVER_ENGINE_H__
#define _CLOVER_ENGINE_H__ 1

#define USE_2D_RENDERER 1
#define ASSETS_PATH(path) std::string("../assets/" + std::string(path))

#include <Clover.h>
#include <Window.h>

#include <Tools/Log.h>
#include <Tools/Timer.h>

#include <Utils/Library.h>

#include <Graphics/renderer.h>
#include <Graphics/renderer2D.h>
#include "Scene/scene.h"
#include "Layers/layer_stack.h"
#include "Scene/Panels/SceneViewportPanel.h"

Delegate_OneParam(OnTickEvent, float, DeltaTime)

class CloverEngine : public Cl_Object
{
public:
	static CloverEngine* GetEngine();
	bool Construct(const WindowProps& Properties, bool UseImGui = false);
	void Run();
	void Shutdown();

	OnTickEvent m_Tick;
	TimerManager m_TimerManager;

protected: // Override by the User
	virtual void OnUserConstruct() {};
	virtual void OnUserUpdate(float fDeltaTime) {};
	virtual void OnUserUI(float fDeltaTime) {};
	virtual void OnUserImGuiUpdate(float fDeltaTime) {};
	virtual void OnUserDestroy() {};

protected:

	glm::vec2 GetMouseScreenPosition();


	// ImGui
	void ImGuiConstruct();
	void ImGuiSetDarkTheme();
	void ImGuiBeginDockspace();
	void ImGuiEndDockspace();
	void ImGuiShowDrawStats();
	void ImGuiNewFrame();
	void ImGuiEndFrame();

private:
	void OnShutdown();


public:
	Ref<Scene> m_MainScene;
	LayerStack m_LayerStack;
	u32 m_MainRenderLayer;

	bool m_Running;
	bool m_ImGuiEnabled;
	Ref<Window> m_Window;
	Clock m_Clock;

private:
	float m_DeltaTime = 0.f;
	bool m_ImGuiConstructed;
};


#endif
