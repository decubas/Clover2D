#ifndef _ECS_H
#define _ECS_H

#include <Clover.h>

namespace ECS
{


	template<typename T>
	ClassID getClassID()
	{
		return std::type_index(typeid(T)).hash_code();
	}

	template<typename T>
	std::string getClassName()
	{
		return std::type_index(typeid(T)).name();
	}

#define COMPONENT_CLASS(type) ClassID getStaticType() override { return getClassID<type>(); } \
									  virtual std::string getName() override { return #type; } \
									  virtual u32 getStaticTypeSize() override {return sizeof(type);}

#define SHOW_IN_INSPECTOR(value) virtual bool showInInspector() override { return value; }
#define IS_REMOVABLE(value) virtual bool isRemovable() override { return value; }




		//Interface
	class Component
	{
	protected:
		friend class Ecs;
		ComponentID id_ = -1; // unique ID
		EntityID owner_ = -1;
	public:

		bool enable_ = true;

		Component() {}
		virtual ~Component()
		{
			owner_ = -1;
		};

		ComponentID getID() { return id_; }
		EntityID getOwnerID() { return owner_; }

		virtual bool isRemovable() { return true; }
		virtual bool showInInspector() { return true; }
		virtual ClassID getStaticType() = 0;
		virtual u32 getStaticTypeSize() = 0;
		virtual std::string getName() = 0;

		bool operator==(const Component& other)
		{
			return other.id_ == id_;
		}
		bool operator!=(const Component& other)
		{
			return other.id_ != id_;
		}

		virtual void ImGuiEditor() {}

		virtual void on_construct() {}
		virtual void on_destruct() {}
	};


	class ElementBuffer
	{
		struct Element
		{
			void* Data = nullptr;
			u32 BytesSize = 0;

			template<class T>
			T* get()
			{
				return (T*)Data;
			}
		};

		Element* Elements;
		u32 BytesSize;
		u32 Count;
		u32 MaxCount;
	public:

		ElementBuffer()
			: Elements(nullptr), BytesSize(0), Count(0)
		{
		}

		ElementBuffer(u32 size)
			: MaxCount(size), BytesSize(size * sizeof(Element)), Count(0)
		{
			allocate(size);
		}

		inline uint32_t sizeInBytes() const { return Count * sizeof(Element); }
		inline uint32_t size() const { return Count; }
		inline uint32_t maxSize() const { return MaxCount; }

#pragma region Funtionality

#define CHECK_PTR(data, returnVal) 	if (data == nullptr) return returnVal;
#define CHECK_INDEX(index, returnVal) 	if (index >= MaxCount) return returnVal;

		void copy(ElementBuffer& other)
		{
			allocate(other.MaxCount);
			memcpy(Elements, other.Elements, other.BytesSize);
			Count = other.Count;
		}

		void safeCopy(ElementBuffer& other)
		{
			allocate(other.MaxCount);

			for (byte i = 0; i < other.MaxCount; i++)
			{
				Element* e = &Elements[i];
				e = createElement(other.Elements[i].Data, other.Elements[i].BytesSize);
			}

			Count = other.Count;
		}

		void allocate(u32 size)
		{
			if (Elements == nullptr) clear();

			if (size == 0)
				return;

			Elements = new Element[size];
			BytesSize = size * sizeof(Element);
		}

		void resize(u32 size)
		{
			if (size <= 0)
			{
				clear();
				return;
			}

			allocate(size);
			memcpy(Elements, Elements, BytesSize);
		}

		void ZeroInitialize()
		{
			Count = 0;
			if (Elements)
				memset(Elements, 0, BytesSize);
		}

		template<typename T>
		T* read(u32 offset = 0)
		{
			CHECK_INDEX(offset, nullptr);
			CHECK_PTR(Elements[offset].Data, nullptr);
			return (T*)(Elements[offset].Data);
		}

		template<typename T>
		T* pop(u32 offset = 0)
		{
			CHECK_INDEX(offset, nullptr);
			CHECK_PTR(Elements[offset].Data, nullptr);
			T* ptr = (T*)Elements[offset].Data;
			Elements[offset].Data = nullptr;
			Elements[offset].BytesSize = 0;
			shiftLeft(offset);
			return ptr;
		}

		template<typename T>
		T* at(u32 offset = 0)
		{
			CHECK_INDEX(offset, nullptr);
			CHECK_PTR(Elements[offset].Data, nullptr);
			return (T*)(Elements[offset].Data);
		}

		static u32 find(ElementBuffer* buf, void* target)
		{
			CHECK_PTR(buf, -1);
			CHECK_PTR(target, -1);
			for (u32 offset = 0; offset < buf->MaxCount; offset++)
			{
				if (buf->Elements[offset].Data == target)
				{
					return offset;
				}
			}
			return -1;
		}

		void destroy(u32 offset)
		{
			CHECK_INDEX(offset);

			delete Elements[offset].Data;
			Elements[offset].BytesSize = 0;
			shiftLeft(offset);
			Count--;
		}


		void* pushBack(void* data, u32 byteSize)
		{
			CHECK_PTR(data, nullptr);

			checkIfResize();

			Element* e = &Elements[Count];
			e = createElement(data, byteSize);
			CHECK_PTR(e, nullptr);

			Count++;
			return e->Data;
		}

		template<class T>
		T* pushBack(T* data)
		{
			CHECK_PTR(target, nullptr);

			checkIfResize();

			Element* e = &Elements[Count];
			e = createElement(data, sizeof(T));
			CHECK_PTR(e, nullptr);

			Count++;
			return (T*)e->Data;
		}

		template<class T>
		T* emplaceBack(T* data)
		{
			CHECK_PTR(data, nullptr);

			checkIfResize();

			Element* e = &Elements[Count];
			e->Data = (void*)data;
			e->BytesSize = sizeof(T);

			Count++;
			return (T*)e->Data;
		}

		void* moveBack(void* data, u32 byteSize)
		{
			CHECK_PTR(data, nullptr);

			checkIfResize();

			Element* e = Elements + Count;
			e->Data = std::move(data);
			e->BytesSize = byteSize;

			Count++;
			return e->Data;
		}

		void clear()
		{
			delete[] Elements;
			Elements = nullptr;
		}

		operator bool() const
		{
			return Elements != nullptr;
		}

		void* at(int index)
		{
			CHECK_INDEX(index, nullptr);
			return Elements[index].Data;
		}

		template<class T>
		T* at(int index)
		{
			CHECK_INDEX(index, nullptr);
			return (T*)Elements[index].Data;
		}

		void* operator[](int index)
		{
			CHECK_INDEX(index, nullptr);
			return Elements[index].Data;
		}

		void* operator[](int index) const
		{
			CHECK_INDEX(index, nullptr);
			return Elements[index].Data;
		}


		template<class T, typename F>
		void foreach(F& func)
		{
			for (u32 offset = 0; offset < Count; offset++)
			{
				func((T*)Elements[offset].Data);
			}
		}

		template<class T, typename F>
		void foreachBool(F& func)
		{
			for (u32 offset = 0; offset < Count; offset++)
			{
				if (func((T*)Elements[offset].Data))
				{
					return;
				}
			}
		}

#pragma endregion Funtionality

#pragma region Iterator

		struct Iterator
		{
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;

			using value_type = Element;
			using pointer = value_type*;  // or also value_type*
			using reference = value_type&;  // or also value_type&

			Iterator(pointer ptr) : m_ptr(ptr) {}

			reference operator*() const { return *m_ptr; }
			pointer operator->() { return m_ptr; }

			// Prefix increment
			Iterator& operator++() { m_ptr++; return *this; }

			// Postfix increment
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

			friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
			friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

		private:

			pointer m_ptr;
		};

		Iterator begin() { return Iterator(Elements); }
		Iterator end() { return Iterator(&Elements[Count]); }
#pragma endregion Iterator

	private:

		void checkIfResize()
		{
			if (Count * sizeof(Element) >= BytesSize)
			{
				resize((u32)((float)BytesSize * 1.5f));
			}
		}

		void shiftLeft(u32 index)
		{
			Element* current = &Elements[index];

			for (u32 i = index + 1; i < Count; i++)
			{
				Element* next = &Elements[i];

				current->Data = next->Data;
				current->BytesSize = next->BytesSize;

				next->Data = nullptr;
				next->BytesSize = 0;

				current = next;
			}
		}


		void shiftRight(u32 index)
		{
			Element* current = &Elements[Count];

			for (u32 i = Count - 1; i >= index; --i)
			{
				Element* left = &Elements[i];

				current->Data = left->Data;
				current->BytesSize = left->BytesSize;

				left->Data = nullptr;
				left->BytesSize = 0;

				current = left;
			}
		}

		Element* createElement(void* data, u32 bytesSize)
		{
			CHECK_PTR(data, nullptr);
			Element* e = new Element();
			memcpy(e->Data, data, bytesSize);
			e->BytesSize = bytesSize;
			return e;
		}
	};

	class Ecs
	{
	public:

		Ecs()
		{
			Clear();
		}
		~Ecs()
		{
			Clear();
		}


		friend class Component;
		// Creates an Entity of the given class T
		// Checks if there is already a pool for that Entity type T, if not then it creates one
		EntityID CreateEntity()
		{
			entities_.push_back(newEntID_);
			entt_components_[newEntID_] = std::vector<ComponentID>();
			++newEntID_;

			++currentEntities_;

			return entities_[newEntID_ - 1];
		}


		void destroy(EntityID id)
		{
			if (!exists(id))
			{
				return;
			}

			for (auto component : entt_components_[id])
			{
				destroyComponent(component);
			}

			entt_components_.erase(id);
		}

		// checks if the given EntityID is a valid entity
		bool exists(EntityID id)
		{
			return entt_components_.find(id) != entt_components_.end();
		}

		std::vector<EntityID> getEntities() noexcept
		{
			return entities_;
		}


		///////////////////////// COMPONENT



		// Creates a components of the given class T and sets the its owner
		// Checks if there is already a pool for that component, if not then it creates one
		template<class T, typename ... Args>
		T* addComponent(EntityID owner, Args&& ... args)
		{
			ClassID type(getClassID<T>());
			T* newComp;


			if (component_mem_.find(type) == component_mem_.end())
			{
				component_mem_[type] = new ElementBuffer(5);
			}

			newCompID_++;
			newComp = component_mem_[type]->emplaceBack<T>(new T((Args)(args)...));
			newComp->id_ = newCompID_;
			newComp->owner_ = owner;

			entt_components_[owner].push_back(newCompID_);
			components_[newCompID_] = newComp;

			currentComponents_++;



			newComp->on_construct();
			return newComp;
		}

		// Used for getting the component interface with the given ComponentID
		Component* getComponent(ComponentID id)
		{
			return getComponent<Component>(id);
		}

		// Used for getting the component T with the given ComponentID
		template<class T>
		T* getComponent(ComponentID id)
		{
			if (!existsComponent(id))
			{
				printf("ComponentID: %d does NOT EXIST -- getComponent()", id);
				return nullptr;
			}
			return (T*)components_[id];
		}


		// Reference to component T from the given EntityID
		template<class T>
		T* getFrom(EntityID id)
		{
			if (!exists(id))
			{
				printf("EntityID: %d  does NOT EXIST -- getFrom()", id);
				return nullptr;
			}

			ClassID type(getClassID<T>());
			for (ComponentID compID : entt_components_[id])
			{
				if (components_[compID]->getStaticType() == type)
				{
					return (T*)components_[compID];
				}
			}
			return nullptr;
		}

		std::vector<ComponentID>& getComponentsFrom(EntityID id)
		{
			if (!exists(id))
			{
				printf("EntityID: %d does NOT EXIST -- getComponentsFrom()", id);
			}
			else
			{
				return entt_components_[id];
			}
			return std::vector<ComponentID>();
		}

		template<class A, class B>
		std::tuple<A*, B*> getFrom(EntityID id)
		{
			return std::make_tuple(getFrom<A>(id), getFrom<B>(id));
		}

		template<class T, typename F>
		void forEach(F& func)
		{
			if (existsComponent<T>()) getByType<T>().foreach<T>(func);

		}

		template<class T, typename F>
		void forEachBool(F& func)
		{
			if (existsComponent<T>()) getByType<T>().foreachBool<T>(func);
		}

		// Get a reference to all Components of type T from the pool
		template<class T>
		ElementBuffer& getByType() noexcept
		{
			ClassID temp_id = std::type_index(typeid(T)).hash_code();
			if (existsComponent<T>())
			{
				return *component_mem_[temp_id];
			}
			assert(false);
			return ElementBuffer(0);
		}

		// Checks if a pool of components T exists
		template<class T>
		bool existsComponent()
		{
			ClassID temp_id = std::type_index(typeid(T)).hash_code();
			if (component_mem_.find(temp_id) != component_mem_.end())
			{
				return true;
			}
			return false;
		}

		bool existsComponent(ComponentID id)
		{
			return components_.find(id) != components_.end();
		}

		void destroyComponent(ComponentID id)
		{
			if (!existsComponent(id))
			{
				printf("ComponentID: %d does NOT EXIST -- destroyComponent()", id);
				return;
			}

			Component* comp = getComponent(id);
			comp->on_destruct();

			entt_components_[comp->owner_].erase(std::find(entt_components_[comp->owner_].begin(), entt_components_[comp->owner_].end(), id));

			comp->owner_ = -1;

			ClassID type = comp->getStaticType();
			component_mem_[type]->destroy(ElementBuffer::find(component_mem_[type], (void*)comp));

			if (component_mem_[type]->size() == 0)
			{
				component_mem_[type]->clear();
				delete component_mem_[type];
				component_mem_.erase(type);
			}
			components_[id] = nullptr;
			components_.erase(id);
		}

		void Clear()
		{
			//component_pools_.clear();
			for (auto p : entt_components_)
			{
				p.second.clear();
			}
			entt_components_.clear();
			entities_.clear();

			for (auto p : component_mem_)
			{
				if (p.second != nullptr)
					delete p.second;

				p.second = nullptr;
			}
			component_mem_.clear();

			currentComponents_ = 0;
			currentEntities_ = 0;
			newCompID_ = 0;
			newEntID_ = 0;
		}

		const ComponentID GetLastComponentID() const { return newCompID_ - 1; }
		const EntityID GetLastEntityID() const { return newEntID_ - 1; }

		void copyComponent(Component* other)
		{
			Component* newComp = (Component*)malloc(other->getStaticTypeSize());
			memcpy_s(newComp, other->getStaticTypeSize(), other, other->getStaticTypeSize());
			if (component_mem_.find(other->getStaticType()) == component_mem_.end())
			{
				component_mem_[other->getStaticType()] = new ElementBuffer(5);
			}
			component_mem_[other->getStaticType()]->pushBack((void*)newComp, other->getStaticTypeSize());
		}

		void Copy(const Ecs& other)
		{
			entities_ = other.entities_;

			for (auto p : other.entt_components_)
			{
				entt_components_[p.first] = std::vector<ComponentID>();
				entt_components_[p.first].swap(p.second);
			}
			//entt_components_.swap(other.entt_components_);

			for (auto c : other.component_mem_)
			{
				component_mem_[c.first] = new ElementBuffer(5);
				component_mem_[c.first]->copy(*c.second);
			}

			owner_ = other.owner_;
			currentComponents_ = other.currentComponents_;
			newCompID_ = other.newCompID_;
			currentEntities_ = other.currentEntities_;
			newEntID_ = other.newEntID_;
		}

	private:
		std::vector<EntityID> entities_;
		std::unordered_map<EntityID, std::vector<ComponentID>> entt_components_;

		std::unordered_map<ClassID, ElementBuffer*> component_mem_;
		std::unordered_map<ComponentID, Component*> components_;


		void* owner_ = nullptr;
		u32 currentComponents_ = 0;
		ComponentID newCompID_ = 0;

		u32 currentEntities_ = 0;
		EntityID newEntID_ = 0;
	};




}

#endif