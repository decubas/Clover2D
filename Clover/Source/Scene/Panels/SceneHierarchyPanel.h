#ifndef SCENE_HIERARCHY_PANEL_H_
#define SCENE_HIERARCHY_PANEL_H_ 1

#include <Clover.h>

class Scene;
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);
		~SceneHierarchyPanel() {};

		void SetContext(const Ref<Scene>& context);

		void SetSelectedObject(UniqueID id);
		UniqueID* GetSelectedObject() { return m_SelectedObject; }
		void OnImGuiRender();
		bool IsSelectedEntityValid() { return m_SelectedObject >= 0; }

	private:
		void DrawObjectNode(UniqueID uniqueId);
		void DrawComponents();

	protected:
		Ref<Scene> m_Context;
		UniqueID* m_SelectedObject = nullptr;
	};

#endif
