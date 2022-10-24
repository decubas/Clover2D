#include "Clover.h"

#include <yaml-cpp/yaml.h>

#include <Utils/platformUtils.h>
#include <Tools/Serializer.h>

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}


	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}



///// JSON
	const char openObject = '{';
	const char closeObject = '}';

	const char openName = '"';

	const char changeToValue = ':';
	const char changeToKey = ',';

	void JsonSerializer::Print(const JsonObject& object)
	{
		std::cout << object.m_Key<< " : " << std::endl;
		for (auto& pair : object.m_KeyValues)
		{
			std::cout << "    " << pair.first << " : " << pair.second << std::endl;
		}
		for (auto& pair : object.m_Childs)
		{
			Print(pair.second);
		}
	}

	JsonSerializer::JsonObject JsonSerializer::Load(const char* filepath)
	{
		std::ifstream file_(filepath);
		std::ostringstream temp_data;
		temp_data << file_.rdbuf();
		std::string data = temp_data.str();
		JsonObject root;
		Process(data, root);
		return root;
	}

	void JsonSerializer::Write(const char* filepath, const JsonObject& object)
	{
		std::fstream file(filepath);
		file << openObject << std::endl;
		Write(file, object, 1);
		file << closeObject << std::endl;

		file.close();
	}

	void JsonSerializer::Write(std::fstream& file, const JsonObject& object, const int depth)
	{
		int it = 0;
		for (auto& pair : object.m_KeyValues)
		{
			for (int i = 0; i <= depth; ++i)
				file << " ";

			file << openName << pair.first << openName << ": " << openName << pair.second << openName;

			++it;

			if(it < object.m_KeyValues.size() || object.m_Childs.size() > 0)
				file << ",";

			file << std::endl;
		}

		it = 0;
		for (auto& pair : object.m_Childs)
		{
			file << openName << pair.first << openName << ": " << openObject << std::endl;
			Write(file, pair.second, depth + 1);
			file << closeObject;

			++it;
			if (it < object.m_Childs.size())
				file << ",";

			file << std::endl;
		}
	}

	void JsonSerializer::Process(std::string& data, JsonObject& root)
	{
		std::string::iterator iterator = data.begin();

		root = ProcessObject(data, root, iterator);
		root.m_Key = "Root";

		data.clear();
		return;
	}

	JsonSerializer::JsonObject JsonSerializer::ProcessObject(std::string& data, JsonObject& parent,
		std::string::iterator& iterator)
	{
		std::string key, value;
		bool begin_name = false, is_Value = false, is_Key = true;
		JsonObject tempObject;
		JsonObject tempChild;
		for (; iterator != data.end(); ++iterator)
		{
			char current = *iterator;

			switch (current)
			{
			case openObject:
				if (is_Value)
				{
					// Found a child, time to read it!
					tempChild = ProcessObject(data, tempObject, ++iterator);
					tempChild.m_Key = key;
					tempObject.m_Childs.insert({ key, tempChild });
				}
				break;

			case closeObject:
				// Exit this object
				return tempObject;
				break;

			case openName:
				if (is_Key)
				{
					key = GetName(data, ++iterator);
				}
				if (is_Value)
				{
					value = GetName(data, ++iterator);
					tempObject.m_KeyValues.insert({ key, value });
				}
				break;

			case changeToValue:
				is_Key = false;
				is_Value = true;
				break;

			case changeToKey:
				is_Key = true;
				is_Value = false;
				break;

			default:
				break;
			}

		}
	}

	std::string JsonSerializer::GetName(std::string& data, std::string::iterator& iterator)
	{
		std::string name;
		for (; iterator != data.end(); ++iterator)
		{
			if (*iterator == '"')
			{
				//++iterator;
				return name;
			}

			name += *iterator;
		}
	}

	/*std::string& JsonSerializer::JsonObject::operator[](const std::string& key)
	{
		//Look for a keyValue
		{
			auto it = m_KeyValues.find(key);
			// Found it!
			if (it != m_KeyValues.end())
			{
				return it->second;
			}
		}

		//Look for a Child
		{
			auto it = m_Childs.find(key);
			// Found it!
			if (it != m_Childs.end())
			{
				return it->second[key];
			}
		}

		// No luck
		return "";
	}*/

