
#include "scene_manager.h"
#include <Physics/physics2d_world.h>
#include "CloverEngine.h"

SceneManager* SceneManager::instance = new SceneManager();

Ref<Scene>& SceneManager::GetCurrent()
{
	return m_Scene;
}

void SceneManager::CreateNew()
{
	m_Scene = CreateRef<Scene>();
	m_SceneFilepath = "";
}

void SceneManager::Play()
{
	m_ReloadScene = CreateRef<Scene>();
	m_ReloadScene->Copy(m_Scene);
	SaveScene();
	m_Scene->OnScenePlay();
	for (auto& layer : CloverEngine::GetEngine()->m_LayerStack)
	{
		//layer->OnScenePlay();
	}

}

void SceneManager::Stop()
{
	m_Scene->OnSceneStop();
	for (auto& layer : CloverEngine::GetEngine()->m_LayerStack)
	{
		//layer->OnSceneStop();
	}

	ReloadScene();
}

void SceneManager::ResetScene()
{
	//m_Scene = m_ReloadScene;
	ReloadScene();
	m_Scene->OnScenePlay();
	for (auto& layer : CloverEngine::GetEngine()->m_LayerStack)
	{
		//layer->OnScenePlay();
	}
}

bool SceneManager::OpenScene()
{
	m_SceneFilepath = "";
	std::optional<std::string> filepath = FileDialogs::OpenFile("Coffee Scene (*.coffee)\0*.coffee\0");
	if (filepath)
	{
		OpenScene(*filepath);
		return true;
	}
	return false;
}

bool SceneManager::OpenScene(const std::string& filepath)
{
	Physics2D::getWorld()->Reset();
	m_Scene = CreateRef<Scene>();
	SceneSerializer serializer(m_Scene);
	serializer.Deserialize(filepath);

	m_SceneFilepath = filepath;
	return true;
}

void SceneManager::ReloadScene()
{
	//	m_Scene = m_ReloadScene;
	if (m_SceneFilepath != "")
	{
		Physics2D::getWorld()->Reset();
		m_Scene = CreateRef<Scene>();
		SceneSerializer serializer(m_Scene);
		serializer.Deserialize(m_SceneFilepath);
	}
}

bool SceneManager::SaveScene()
{
	if (m_SceneFilepath != "")
	{
		std::string tempPath = m_SceneFilepath.
			replace(m_SceneFilepath.find_last_of("\\") + 1,
				m_SceneFilepath.find_last_of("."),
				m_Scene->GetName().c_str());
		m_SceneFilepath = tempPath + ".coffee";
		SceneSerializer serializer(m_Scene);
		serializer.Serialize(m_SceneFilepath);
		return true;
	}
	return false;
}

bool SceneManager::SaveSceneAs()
{
	std::optional<std::string> filepath = FileDialogs::SaveFile("Scene (*.clover)\0*.clover\0");
	if (filepath)
	{
		SceneSerializer serializer(m_Scene);
		serializer.Serialize(*filepath);
		return true;
	}
	return false;
}