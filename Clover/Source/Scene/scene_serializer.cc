
#include "scene_serializer.h"
#include "Tools/FileManager.h"
#include "Utils/platformUtils.h"


SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
	: m_Scene(scene)
{

}

void SceneSerializer::Serialize(const std::string& filepath)
{
	//FileManager::WriteBin();
	Cl_Assert(false, "Not implemented");
}

void SceneSerializer::SerializeRuntime(const std::string& filepath)
{
	//NOT IMPLEMENTED		
	Cl_Assert(false, "Not implemented");

}

bool SceneSerializer::Deserialize(const std::string& filepath)
{
	//FileManager::ReadBin();
	Cl_Assert(false, "Not implemented");
	return false;
}

bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
{
	//NOT IMPLEMENTED
	Cl_Assert(false, "Not implemented");
	return false;
}

