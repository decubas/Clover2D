#ifndef _CLOVER_ENGINE_H__
#define _CLOVER_ENGINE_H__ 1

#define USE_2D_RENDERER 1

#include <Clover.h>
#include <Window.h>

#include <Tools/Log.h>
#include <Tools/Timer.h>

#include <Utils/Library.h>

#include <Graphics/renderer.h>
#include <Graphics/renderer2D.h>


class CloverEngine : public Cl_Object
{
public:
	bool Construct(const WindowProps& Properties, bool UseImGui = false);
	void Run();
	void Shutdown();

protected: // Override by the User
	virtual void OnUserConstruct() {};
	virtual void OnUserUpdate(float fDeltaTime) {};
	virtual void OnUserUI(float fDeltaTime) {};
	virtual void OnUserImGuiUpdate(float fDeltaTime) {};
	virtual void OnUserDestroy() {};

protected:

	glm::vec2 GetMouseScreenPosition();

	void OnWindowResize(float Width, float Height);

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


protected:
	u32 m_MainRenderLayer;
	u32 m_UIRenderLayer;
	bool m_Running;
	bool m_ImGuiEnabled;
	Ref<FrameBuffer> m_ScreenBuffer;
	EditorCamera m_EditorCamera;
	SceneCamera m_UICamera;
	Ref<Window> m_Window;
	Clock m_Clock;

private:
	bool m_ImGuiConstructed;
};


#endif
