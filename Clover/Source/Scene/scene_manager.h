#pragma once

#include <Clover.h>
#include "scene.h"
#include "scene_serializer.h"
#include <Utils/PlatformUtils.h>

#define SCENE SceneManager::Get()

class SceneManager
{
private:
	Ref<Scene> m_ReloadScene;
	std::string m_SceneFilepath;
	Ref<Scene> m_Scene;
	static SceneManager* instance;
public:

	inline static SceneManager& Get() { return *instance; }



	Ref<Scene>& GetCurrent();
	void CreateNew();
	void Play();
	void Stop();
	void ResetScene();
	void ReloadScene();
	bool OpenScene();
	bool OpenScene(const std::string& filepath);
	bool SaveScene();
	bool SaveSceneAs();
};
