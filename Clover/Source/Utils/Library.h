#ifndef _LIBRARY_H_
#define _LIBRARY_H_


#include <Clover.h>

#include <Tools/log.h>

	template<class T>
	class Library : public Cl_Object
	{
	public:

		void Add(const CString& name, const Ref<T>& item)
		{
			if (Exists(name))
			{
				Cl_Warning("Item %s already exists", name.c_str());
				return;
			}
			Cl_Info("Item %s Added Successfully", name.c_str());
			items_[name] = item;
			//std::move al pool
		}

		virtual Ref<T> Load(const CString& name, const Ref<T>& item)
		{
			if (Exists(name))
			{
				Cl_Warning("Item %s already exists", name.c_str());
				return items_[name];
			}

			Add(name, item);

			return items_[name];
		}

		Ref<T> Get(const CString& name)
		{
			if (!Exists(name))
			{
				Cl_Error("Item %s does not exist", name.c_str());
				Cl_Assert(false, "");
			}
			return items_[name];
		}

		void Show()
		{
			Cl_Info("--------- %s Library ---------", std::type_index(typeid(T)).name());
			for (auto& item : items_)
			{
				Cl_Info(item.first);
			}
		}

		bool Exists(const CString& name) const
		{
			return  items_.find(name) != items_.end();;
		}

	protected:
		//Memory Pool -- TO IMPLEMENT
		//CMap<ClassID, Ref<IPool>> pools_;

		// (Name, Item)
		CMap<CString, Ref<T>> items_;
	};
#endif