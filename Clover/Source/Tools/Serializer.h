#ifndef __SERIALIZER__H
#define __SERIALIZER__H 1

#include <Clover.h>

class Encoder
{
public:
	template <class T, class R>
	static R Encode(T value)
	{
		return R();
	}

	static std::string Encode(const std::string value) { return value; };
	static std::string Encode(const char value) { return std::to_string(value); };
	static std::string Encode(const unsigned char value) { return std::to_string(value); };
	static std::string Encode(const int value) { return std::to_string(value); };
	static std::string Encode(const unsigned int value) { return std::to_string(value); };
	static std::string Encode(const float value) { return std::to_string(value); };
	static std::string Encode(const double value) { return std::to_string(value); };

	template <class T, class R>
	static bool Decode(T& outValue, R& value)
	{
		return true;
	}

	static bool Decode(std::string& outValue, std::string value) { return true; };
	static bool Decode(char& outValue, std::string value) { outValue = stoi(value); return true; };
	static bool Decode(unsigned char& outValue, std::string value) { outValue = stoi(value); return true; };
	static bool Decode(int& outValue, std::string value) { outValue = stoi(value); return true; };
	static bool Decode(unsigned int& outValue, std::string value) { outValue = stoi(value); return true; };
	static bool Decode(float& outValue, std::string value) { outValue = stof(value); return true; };
	static bool Decode(double& outValue, std::string value) { outValue = stod(value); return true; };
};


/*
struct Foo2
{
	int a = 10, b = 11, c = 12;
};

class Foo : public Class<Foo>
{
public:

	PROPERTY(int, a);
	Foo2 a;

	PROPERTY(int, b);
	int b = 2;

	PROPERTY(int, c);
	int c = 3;

};

STATIC_PROPERTY(Foo, c);


	printf("Class num prop %d\n", Foo::PropertyMap.size());
	printf("Offset %d %d %d\n", Foo::PropertyMap["a"].GetOffset(), Foo::PropertyMap["b"].GetOffset(), Foo::PropertyMap["c"].GetOffset());
	printf("Data %d %d %d\n", Foo::GetValue<int>(&f, "a"), Foo::GetValue<int>(&f, "b"), Foo::GetValue<int>(&f, "c"));
	printf("Data as string %s \n", f.Property_b.ToString().c_str());
	
	JsonSerializer::JsonObject json = Serializer::Serialize<Foo>(&f, "f");
	JsonSerializer::Print(json);



*/


class ClassBase;

class StaticProperty
{
	friend ClassBase;

public:
	StaticProperty() = default;

	StaticProperty(const std::string& VarName, uint16_t offset)
		: Name(VarName), Offset(offset)
	{}



	uint16_t GetOffset() const { return Offset; }
	std::string GetName() const { return Name; }

protected:
	std::string Name = "";
	uint16_t Offset = 0;
};

class BaseProperty
{
public:
	virtual std::string ToString() { return ""; }
	virtual std::string ToStringStatic(ClassBase* Base) { return ""; }
};



namespace ClassHelpers
{
	static uint16_t PtrToOff(void* Base, void* Ptr) { return (uint16_t)Ptr - (uint16_t)Base; }

	template<typename T>
	static T* OffToPtr(ClassBase* Base, uint16_t offset)
	{
		unsigned char* bytePtr = reinterpret_cast<unsigned char*>(Base);
		bytePtr += offset;
		return (T*)(bytePtr);
	}
}


class ClassBase
{
	friend StaticProperty;


public:

	template<typename T>
	static T& GetValue(ClassBase* Base, const std::string& Name)
	{
		return *ClassHelpers::OffToPtr<T>(Base, PropertyMap[Name].GetOffset());
	}

	static inline std::unordered_map<std::string, StaticProperty> PropertyMap;
	std::unordered_map<std::string, BaseProperty*> LocalPropertyMap;

	void AddLocalProperty(const std::string& Name, BaseProperty* Prop)
	{
		LocalPropertyMap.insert({ Name, Prop });
	}

	static void AddProperty(const std::string& Name, size_t offset)
	{
		PropertyMap.insert({ Name, StaticProperty(Name, offset) });
	};
};

template<typename T>
class Property : public BaseProperty
{
public:

	Property(ClassBase* Base, size_t offset, const std::string& VarName)
		: Owner(Base), Name(VarName)
	{
		Base->AddProperty(VarName, offset);
		Base->AddLocalProperty(VarName, this);
	}

	virtual std::string ToString() override
	{
		return Encoder::Encode(ClassBase::GetValue<T>(Owner, Name));
	}

	T& Get() { return ClassBase::GetValue<T>(Owner, Name); }


private:
	ClassBase* Owner;
	std::string Name;
};

template<typename T, typename classT>
class StaticMemberProperty : public BaseProperty
{
public:

	StaticMemberProperty(size_t offset, const std::string& VarName)
	{
		classT::AddProperty(VarName, offset);
	}

	virtual std::string ToStringStatic(ClassBase* Base) override
	{
		return Encoder::Encode(ClassBase::GetValue<T>(Base, Name));
	}

	T& Get(ClassBase* Base) { return ClassBase::GetValue<T>(Base, Name); }

};

template<typename T>
class Class : public ClassBase
{

public:
	static size_t getClassID() { return std::type_index(typeid(T)).hash_code(); }
	static std::string TypeToString() { return std::type_index(typeid(T)).name(); }
};


#define PROPERTY(type, name) Property<type> Property_##name = Property<type>(this, ClassHelpers::PtrToOff(this, &name), #name);
#define STATIC_PROPERTY(classT,type, name) static inline StaticMemberProperty<type, classT> Static__##classT##__##name = StaticMemberProperty<type, classT>(offsetof(class classT, name), #name);






class JsonSerializer
{
public:
	typedef std::pair<std::string, std::string> KeyValue;
	typedef std::unordered_map<std::string, std::string> KeyValueChildren;

	struct JsonObject
	{
		std::string m_Key;
		KeyValueChildren m_KeyValues;
		std::unordered_map<std::string, JsonObject> m_Childs;
	};
	static void Print(const JsonObject& object);

	static JsonObject Load(const char* filepath);
	static void Write(const char* filepath, const JsonObject& object);

protected:
	static void Write(std::fstream& file, const JsonObject& object, const int depth = 0);
	static void Process(std::string& data, JsonObject& root);
	static JsonObject ProcessObject(std::string& data, JsonObject& parent, std::string::iterator& iterator);
	static std::string GetName(std::string& data, std::string::iterator& iterator);

};


namespace Serializer
{
	template<typename T>
	JsonSerializer::JsonObject Serialize(const Class<T>* Base, const std::string& name)
	{
		JsonSerializer::JsonObject json;
		json.m_Key = name;
		for (const auto& Pair : Base->LocalPropertyMap)
		{
			json.m_KeyValues.insert({ Pair.first, Pair.second->ToString() });
		}

		return json;
	}
}


#endif