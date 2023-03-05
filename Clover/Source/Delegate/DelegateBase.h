#ifndef _DelegateBase_H_
#define _DelegateBase_H_ 1

#include <vector>
#include <memory>
#include <map>

	/////////////////////// DELEGATE SYSTEM
	class IDelegateCallbackBase
	{
	public:
	};

	static long long s_NextDelegateHandle = 0;
	struct DelegateHandle
	{
		DelegateHandle()
		{
			id = s_NextDelegateHandle;
			s_NextDelegateHandle++;
		}

		bool operator==(const DelegateHandle& other) const
		{
			return id == other.id;
		}
		bool operator<(const DelegateHandle& other) const
		{
			return id < other.id;
		}

		long long id;
	};

	namespace std {

		template <>
		struct hash<DelegateHandle>
		{
			std::size_t operator()(const DelegateHandle& dh) const
			{
				return hash<uint64_t>()((uint64_t)dh.id);
			}
		};
	}

	typedef std::map<DelegateHandle, std::shared_ptr<IDelegateCallbackBase>> CallbackArrayBase;
	
	class DelegateBase
	{
	public:
		DelegateBase() = default;
		~DelegateBase()
		{
			Clear();
		}

		virtual void Clear()
		{
			actions.clear();
		}

		bool IsBound()
		{
			return actions.size() > 0;
		}


		void RemoveFunction(DelegateHandle handle)
		{
			CallbackArrayBase::iterator position = actions.find(handle);
			if (position == actions.end())
			{
				return; 
			}
			actions.erase(handle);
		}

	protected:
		CallbackArrayBase actions;
	};


#define FUNC_CONCAT( ... ) __VA_ARGS__

//			actions[handle] = std::make_shared<DelegateCallback_##DelegateName<T>>((T*)owner, function_);\

#define Delegate_Return_AddRemoveMultParams(DelegateName, ReturnType, ParamTypes)\
		template<class T>\
		DelegateHandle AddFunction(void* owner, ReturnType (T::* function_)(ParamTypes))\
		{\
			DelegateHandle handle;\
			actions.insert({handle, std::make_shared<DelegateCallback_##DelegateName<T>>((T*)owner, function_)});\
			return handle;\
		}

#define Delegate_Broadcast_Return_MultParams(DelegateName, ReturnType, ParamNamesAndTypes, ...)\
		ReturnType Broadcast(ParamNamesAndTypes)\
		{\
			CallbackArrayBase tempActions = actions;\
			for (auto& pair : tempActions)\
			{\
				IDelegateCallback_##DelegateName* Func = (IDelegateCallback_##DelegateName*)pair.second.get();\
				return (*Func).Broadcast(__VA_ARGS__);\
			}\
		}

#define Delegate_Broadcast_MultParams(DelegateName, ParamNamesAndTypes, ...)\
		void Broadcast(ParamNamesAndTypes)\
		{\
			CallbackArrayBase tempActions = actions;\
			for (auto& pair : tempActions)\
			{\
				IDelegateCallback_##DelegateName* Func = (IDelegateCallback_##DelegateName*)pair.second.get();\
				if(Func)\
				{\
					(*Func).Broadcast(__VA_ARGS__);\
				}\
			}\
		}

#define Delegate_Return_MultParam(DelegateName, ReturnType, ParamTypes, ParamNamesAndTypes, ...)\
	class IDelegateCallback_##DelegateName : public IDelegateCallbackBase\
	{\
	public:\
		virtual ReturnType Broadcast(ParamNamesAndTypes) = 0;\
	};\
	template<class T>\
	class DelegateCallback_##DelegateName : public IDelegateCallback_##DelegateName\
	{\
	public:\
		typedef ReturnType (T::* DelegateFunc)(ParamTypes);\
		DelegateCallback_##DelegateName(T* instance, DelegateFunc function_)\
			: instance(instance), function(function_) {}\
		virtual ReturnType Broadcast(ParamNamesAndTypes) override { return (instance->*function)(__VA_ARGS__); }\
		bool operator == (IDelegateCallback_##DelegateName* other)\
		{\
			DelegateCallback_##DelegateName* otherDelegateCallback = (DelegateCallback_##DelegateName*)other;\
			if (otherDelegateCallback == nullptr)\
				return false;\
				return 	(this->function == otherDelegateCallback->function) &&\
				(this->instance == otherDelegateCallback->instance);\
		}\
	private:\
		DelegateFunc function;\
		T* instance;\
	};\
	class DelegateName : public DelegateBase\
	{\
	public:\
		Delegate_Return_AddRemoveMultParams(DelegateName, ReturnType, ParamTypes)\
		Delegate_Broadcast_Return_MultParams(DelegateName, ReturnType, ParamNamesAndTypes, __VA_ARGS__)\
	};

#define Delegate_MultParam(DelegateName, ParamTypes, ParamNamesAndTypes, ...)\
	class IDelegateCallback_##DelegateName : public IDelegateCallbackBase\
	{\
	public:\
		virtual void Broadcast(ParamNamesAndTypes) = 0;\
	};\
	template<class T>\
	class DelegateCallback_##DelegateName : public IDelegateCallback_##DelegateName\
	{\
	public:\
		typedef void (T::* DelegateFunc)(ParamTypes);\
		DelegateCallback_##DelegateName(T* instance, DelegateFunc function_)\
			: instance(instance), function(function_) {}\
		virtual void Broadcast(ParamNamesAndTypes) override { (instance->*function)(__VA_ARGS__); }\
		bool operator == (IDelegateCallback_##DelegateName* other)\
		{\
			DelegateCallback_##DelegateName* otherDelegateCallback = (DelegateCallback_##DelegateName*)other;\
			if (otherDelegateCallback == nullptr)\
				return false;\
				return 	(this->function == otherDelegateCallback->function) &&\
				(this->instance == otherDelegateCallback->instance);\
		}\
	private:\
		DelegateFunc function;\
		T* instance;\
	};\
	class DelegateName : public DelegateBase\
	{\
	public:\
		Delegate_Return_AddRemoveMultParams(DelegateName, void, ParamTypes)\
		Delegate_Broadcast_MultParams(DelegateName, ParamNamesAndTypes, __VA_ARGS__)\
	};




#define Delegate(DelegateName) Delegate_MultParam(DelegateName, void, void, )
#define Delegate_Return(DelegateName, ReturnType) Delegate_Return_MultParam(DelegateName, ReturnType, void, void, )

#define Delegate_OneParam(DelegateName, ParamT1, Param1) Delegate_MultParam(DelegateName, ParamT1, FUNC_CONCAT(ParamT1 Param1), Param1)
#define Delegate_TwoParams(DelegateName, ParamT1, Param1, ParamT2, Param2) Delegate_MultParam(DelegateName, FUNC_CONCAT(ParamT1, ParamT2), FUNC_CONCAT(ParamT1 Param1, ParamT2 Param2), Param1, Param2)

#define Delegate_ReturnOneParam(DelegateName, ReturnType, ParamT1, Param1) Delegate_Return_MultParam(DelegateName, ReturnType, ParamT1, FUNC_CONCAT(ParamT1 Param1), Param1)
#define Delegate_ReturnTwoParams(DelegateName, ReturnType, ParamT1, Param1, ParamT2, Param2) Delegate_Return_MultParam(DelegateName, ReturnType, FUNC_CONCAT(ParamT1, ParamT2), FUNC_CONCAT(ParamT1 Param1, ParamT2 Param2), Param1, Param2)

#endif
