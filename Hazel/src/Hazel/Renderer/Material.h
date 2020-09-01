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
		void SetFlag(MaterialFlag flag, bool value);
		void SetFlag(const std::string& name, const Ref<TextureCube>& skybox);
	};

}
