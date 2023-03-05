
#include <Clover.h>
#include "scene.h"
#include <Graphics/render_command.h>
#include <Graphics/renderer2D.h>



	Scene::~Scene()
	{
		Cl_Assert(m_ObjectIDMap.size() == 0, "Objects in Scene not deleted");
	}


	UniqueID Scene::CreateObject(void* Data)
	{
		UniqueID id;
		m_ObjectIDMap[id] = Data;
		return id;
	}


	Cl_Object* Scene::GetDefaultObject(UniqueID id)
	{
		if (!m_ObjectIDMap.count(id)) return nullptr;

		return GetObject<Cl_Object>(id);
	}

	void Scene::OnViewportResize(u32 width, u32 height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		m_EditorCamera.SetViewportSize(width, height);
	}

	void Scene::OnUpdateRuntime(float delta)
	{
		/*MTR_SCOPE("Scene", "Update");

		// Get Current Camera
		if (m_ecs.existsComponent<CameraComponent>())
		{

			m_ecs.forEach<CameraComponent>([&](auto* camera)
				{
					if (camera->primary)
					{
						m_MainCameraTransform = Entity(this, camera->getOwnerID()).getComponent<TransformComponent>().getTransformMatrix();
						m_MainCamera = &camera->Camera;
					}
				});
		}


		// Update all entities
 		if (m_ecs.existsComponent<NativeScriptComponent>())
 		{
 			for (auto& it : m_ecs.getByType<NativeScriptComponent>())
 			{
				auto* script = it.get<NativeScriptComponent>();
				if (!script->bind) continue;

				if (!script->initialized)
				{

					if (script->InstantiateScript)
					{
						if (!script->Instance)
						{
							script->Instance = script->InstantiateScript();
							script->Instance->m_Entity = Entity(this, script->getOwnerID());
							script->Instance->OnCreate();
						}
					}
					else
					{
						script->Instance->m_Entity = Entity(this, script->getOwnerID());
						script->Instance->OnCreate();
					}

					script->initialized = true;
				}


 
 				script->Instance->OnUpdate(delta);
 			}
 		}
		*/
		
	}


	void Scene::OnUpdateEditor(float delta)
	{

	}


	void Scene::OnImGuiRender()
	{
		ImGui::Begin("Scene");
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), m_Name.c_str());
		if (ImGui::InputText("##SceneName", buffer, sizeof(buffer)))
		{
			m_Name = std::string(buffer);
		}
		ImGui::End();

	}

	void Scene::OnScenePlay()
	{
		m_IsPlaying = true;

	}


	void Scene::OnSceneStop()
	{
		m_IsPlaying = false;
	}

