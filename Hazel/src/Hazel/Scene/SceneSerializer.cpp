#include "hzpch.h"
#include "SceneSerializer.h"


namespace Hazel {

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
	{
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		// Not implemented
		HZ_CORE_ASSERT(false);
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		// Not implemented
		HZ_CORE_ASSERT(false);

		return false;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// Not implemented
		HZ_CORE_ASSERT(false);

		return false;
	}

}
