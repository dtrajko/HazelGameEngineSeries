#pragma once

#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/TextureCube.h"


namespace Hazel {

	enum class MaterialFlag {
		DepthTest,
	};

	class Material
	{
	public:
		static Ref<Material> Create(Ref<Shader>);
	};

	class MaterialInstance
	{
	public:
		static Ref<MaterialInstance> Create(Ref<Material>);
		Ref<Shader> GetShader();

		void SetFlag(MaterialFlag flag, bool value);
		void SetFlag(const std::string& name, const Ref<TextureCube>& skybox);

		void Set(const std::string& name, float value);
		void Set(const std::string& name, glm::mat4 value);
		void Set(const std::string& name, glm::vec3 value);
		void Set(const std::string& name, Ref<TextureCube> textureCube);
		void Set(const std::string& name, Ref<Texture2D> texture2D);

	};

}
