#ifndef __SERIALIZER__H
#define __SERIALIZER__H 1

#include <Clover.h>


#define SERIALIZE_CHILD(variable) m_Serialized.m_Childs[#variable] = variable.Encode();
#define SERIALIZE_VAR(variable) m_Serialized.m_KeyValues[#variable] = Encoder::Encode(variable);
#define DESERIALIZE_VAR(variable) Encoder::Decode(variable, m_Serialized.m_KeyValues[#variable]);
#define DESERIALIZE_CHILD(variable) variable.Decode(m_Serialized.m_Childs[#variable]);
#define SERIALIZED_VAR_DEFAULT(key, value) {#key, #value}
#define SERIALIZED_VAR(key) SERIALIZED_VAR_DEFAULT(key, )
#define SERIALIZED_CLASS(ClassName, serialized, deserialized, ...) \
JsonSerializer::JsonObject m_Serialized = \
{					\
#ClassName,			\
 { __VA_ARGS__ },	\
{}					\
};					\
JsonSerializer::JsonObject& Serialize()	\
{					\
	serialized;		\
return m_Serialized;\
};					\
void Deserialize()	\
{					\
	deserialized;	\
};					\
JsonSerializer::JsonObject& Encode()\
{					\
	Serialize();	\
	return m_Serialized;\
}					\
bool Decode(JsonSerializer::JsonObject& object)\
{					\
	m_Serialized = object;\
	Deserialize();	\
	return true;	\
}

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

	protected:

	};
#endif