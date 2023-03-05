#include "SceneHierarchyPanel.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "../scene.h"


	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}


	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectedObject = nullptr;
	}


	void SceneHierarchyPanel::SetSelectedObject(UniqueID id)
	{
		if (id >= 0)	m_SelectedObject = &id;
		else m_SelectedObject = nullptr;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		for (auto& object : m_Context->m_ObjectIDMap)
		{
			DrawObjectNode(object.first);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectedObject = nullptr;
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectedObject)
		{
			Cl_Object* object = m_Context->GetDefaultObject(*m_SelectedObject);
			object->ShowEditor();
		}
		ImGui::End();

	}



	void SceneHierarchyPanel::DrawObjectNode(UniqueID uniqueId)
	{
		if (uniqueId <= 0) return;

		/*Entity entity(m_Context, entityID);
		std::string& tag = entity.getComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = (m_SelectedObject == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((void*)entityID, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectedObject = entity.getInternalID();
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
			}
			ImGui::EndPopup();
		}

		if (opened)
		{
			if (entity.hasComponent<ChildComponent>())
			{
				for (auto child : entity.getComponent<ChildComponent>().get())
				{
					DrawObjectNode(child);
				}
			}
			ImGui::TreePop();
		}


		if (entityDeleted)
		{
			if (m_SelectedObject == entity)
			{
				m_SelectedObject = -1;
			}
			m_Context->DestroyEntity(entity);
		}*/
	}


	void SceneHierarchyPanel::DrawComponents()
	{
		/*Entity entt(m_Context, m_SelectedObject);
		for (auto compID : entt.getAllComponents())
		{
			ECS::Component* comp = m_Context->m_ecs.getComponent(compID);
			if (comp->showInInspector())
			{
				ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

				bool removeComponent = false;
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth
					| ImGuiTreeNodeFlags_AllowItemOverlap;

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				ImGui::Separator();
				bool open = ImGui::TreeNodeEx((void*)comp, flags, comp->getName().c_str());
				ImGui::PopStyleVar();

				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
				if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
				{
					ImGui::OpenPopup(std::string("ComponentSettings" + comp->getID()).c_str());
				}


				if (ImGui::BeginPopup(std::string("ComponentSettings" + comp->getID()).c_str()))
				{
					if (comp->isRemovable())
					{
						if (ImGui::MenuItem("Remove Component"))
						{
							removeComponent = true;
						}
					}

					ImGui::EndPopup();
				}

				if (open)
				{
					comp->ImGuiEditor();
					ImGui::TreePop();
				}
			}
		}*/
	}
