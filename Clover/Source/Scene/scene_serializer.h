#ifndef __SCENE_SERIALIZER__H
#define __SCENE_SERIALIZER__H 1


#include <Clover.h>
#include "scene.h"


class SceneSerializer
{
public:
	SceneSerializer(const Ref<Scene>& scene);
	~SceneSerializer() = default;


	void Serialize(const std::string& filepath);
	void SerializeRuntime(const std::string& filepath);

	bool Deserialize(const std::string& filepath);
	bool DeserializeRuntime(const std::string& filepath);



private:
	Ref<Scene> m_Scene;
};

#endif