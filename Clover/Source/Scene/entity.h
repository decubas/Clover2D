#ifdef _ENTITY_H__
#define _ENTITY_H__ 1

#include <core.h>
#include <types.h>

#include "engine/scene/scene.h"

namespace Coffee
{


	class Entity
	{
		TransformComponent* transform = nullptr;
	public:
		Entity() = default;

		Entity(Ref<Scene> scene, EntityID id)
			: m_Scene(scene.get()), id_(id)
		{}
		Entity(Scene* scene, EntityID id)
			: m_Scene(scene), id_(id)
		{}

		virtual ~Entity() = default;

		inline const EntityID getInternalID() const { return id_; }
		inline EntityID& getInternalID() { return id_; }

		CID& getCID();

		Scene* getScene()
		{
			return m_Scene;
		}

		template<class T>
		bool hasComponent()
		{
			return m_Scene->m_ecs.getFrom<T>(id_) != nullptr;
		}

		template<class T, typename ... Args>
		T& addComponent(Args&& ... args)
		{
			return *m_Scene->m_ecs.addComponent<T>(id_, (args)...);
		}

		// Only removes the component, Destroy is not called
		void removeComponent(ComponentID id)
		{
			m_Scene->DestroyComponent(id);
		}


		TransformComponent& Transform()
		{
			if (transform == nullptr) transform = &getComponent<TransformComponent>();

			return *transform;
		}

		std::vector<ComponentID>& getAllComponents()
		{
			return m_Scene->m_ecs.getComponentsFrom(id_);
		}

		template<class T>
		T& getComponent()
		{
			return *m_Scene->m_ecs.getFrom<T>(id_);
		}

		template<class T>
		T* getPtrComponent()
		{
			return m_Scene->m_ecs.getFrom<T>(id_);
		}

		bool operator==(const Entity& other)
		{
			return other.id_ == id_;
		}
		bool operator!=(const Entity& other)
		{
			return other.id_ != id_;
		}
		operator bool() { return id_ != -1; }
		operator EntityID() const { return (EntityID)id_; }

	protected:
		Scene* m_Scene;
		EntityID id_ = -1;
	};

}
#endif
