#ifndef _SCENE_H_
#define _SCENE_H_

#include <Clover.h>
#include <imgui.h>
#include <Graphics/camera.h>
#include <Tools/UniqueId.h>
#include "Graphics/renderer2D.h"


	struct SceneData
	{
		CMat4 viewProyectionMatrix_;
	};

	using ObjectMap = std::unordered_map<UniqueID, void*>;

	// Container of id with a pointer to some data
	// Stores camera information
	class Scene : public Cl_Object
	{
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	public:
		
		void Copy(Ref<Scene>& other)
		{
			m_ObjectIDMap = other->m_ObjectIDMap;
			m_Name = other->m_Name;
			m_ViewportHeight = other->m_ViewportHeight;
			m_ViewportWidth = other->m_ViewportWidth;
		}

		Scene(const std::string& name = "Untitled Scene")
			: m_Name(name)
		{};
		~Scene();

		
		UniqueID CreateObject(void* Data);

		template<class T>
		T* GetObject(UniqueID id);

		Cl_Object* GetDefaultObject(UniqueID id);
		
		template<class T>
		T* DestroyObject(UniqueID id);

		// Destroys and removes the component from the entity
		//void DestroyComponent(ComponentID id);
		//void DestroyComponent(ECS::Component* component);

		void OnViewportResize(u32 width, u32 height);

		//Runtime
		void OnUpdateRuntime(float delta);
		//Editor
		void OnUpdateEditor(float delta);

		//Entity FindEntityByTag(const std::string& tag);

		bool IsCameraValid() { return m_MainCamera != nullptr; }

		Camera& GetCurrentCamera() { return *m_MainCamera; }
		glm::mat4& GetCurrentCameraTransform() { return m_MainCameraTransform; }

		//Entity GetPrimaryCameraEntity();
		void OnImGuiRender();

		const ObjectMap& GetObjectMap() const { return m_ObjectIDMap; }

		std::string& GetName() { return m_Name; };
		void SetName(const std::string& name) { m_Name = name; };
		SceneData data_;

		u32 GetViewportWidth() { return m_ViewportWidth; }
		u32 GetViewportHeight() { return m_ViewportHeight; }


		void OnScenePlay();
		void OnSceneStop();

		bool m_IsPlaying = false;
		ObjectMap m_ObjectIDMap;

	private:
		EditorCamera m_EditorCamera;
		u32 m_ViewportHeight = 0;
		u32 m_ViewportWidth = 0;
		std::string m_Name;

		glm::mat4 m_MainCameraTransform = glm::mat4();
		Camera* m_MainCamera = nullptr;
	};

	template<class T>
	T* Scene::GetObject(UniqueID id)
	{
		if (!m_ObjectIDMap.count(id)) return nullptr;

		return (T*)m_ObjectIDMap[id];
	}

	template<class T>
	T* Scene::DestroyObject(UniqueID id)
	{
		if (!m_ObjectIDMap.count(id)) return nullptr;

		T* temp = GetObject(id);
		m_ObjectIDMap.erase(id);
		return temp;
	}

#endif