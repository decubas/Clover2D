#ifndef EDITOR_LAYER_H_
#define EDITOR_LAYER_H_ 1

#include <Clover.h>

#include <Layers/layer.h>
#include <Tools/FileSystem.h>
#include <Graphics/render_pass.h>

#include <Scene/scene_renderer.h>
#include <Scene/Panels/SceneHierarchyPanel.h>
#include <Scene/Panels/SceneViewportPanel.h>

class EditorLayer : public Layer
{
public:
	EditorLayer() {};
	~EditorLayer() {};


	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(float delta) override;
	virtual void OnRender() override;
	virtual void OnImGuiRender() override;

	void OnScenePlay();
	void OnScenePause();
	void OnSceneStop();

	void NewScene();
	void OpenScene();
	void SaveSceneAs();
	void SaveScene();

	void OnKeyPressed(s32 key, s32 Count);
	void OnMouseButtonPressed(s32 key);
private:
	void SetScene();

protected:
	void ReloadScene();
	void Resize();

	Ref<Scene> m_Scene;
	Ref<Scene> m_ReloadScene;

	RenderID renderID;
	
	int m_HoveredEntityID = -1;

	FileSystem m_FileSystem;
	std::string m_SceneFilepath = "";

	EditorCamera m_EditorCamera;
	
	enum class SceneState : u32
	{ Editor = 0, Play = 1, Pause = 2} m_SceneState = SceneState::Editor;

	SceneRenderer m_SceneRenderer;

	bool m_ReloadScriptOnPlay = false;
	bool loadingScene = false;

	// Panels
	SceneHierarchyPanel m_HierarchyPanel;
	SceneViewportPanel m_ViewportPanel;
};

#endif
