#pragma once

#include "Hazel/Core/Timestep.h"
#include "Submesh.h"
#include "Material.h"
#include "VertexArray.h"


namespace Hazel {

	class Mesh
	{
	public:
		void OnUpdate(Timestep ts);
		inline Ref<MaterialInstance> GetMaterialInstance() { return m_MaterialInstance; }

	public:
		Ref<VertexArray> m_VertexArray;
		std::vector<Submesh> m_Submeshes;
		bool m_IsAnimated;
		std::vector<glm::mat4> m_BoneTransforms;
		Ref<Shader> m_MeshShader;

	private:
		Ref<MaterialInstance> m_MaterialInstance;

	};

}
