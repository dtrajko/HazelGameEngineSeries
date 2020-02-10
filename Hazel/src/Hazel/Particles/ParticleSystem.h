#pragma once

#include "Hazel.h"

#include <vector>


namespace Hazel
{
	struct ParticleProps
	{
		glm::vec3 Position;
		glm::vec3 Velocity;
		glm::vec3 VelocityVariation;
		glm::vec3 Rotation;
		glm::vec3 RotationVelocity;
		glm::vec4 ColorBegin;
		glm::vec4 ColorEnd;
		float SizeBegin;
		float SizeEnd;
		float SizeVariation;
		float LifeTime = 1.0f;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem();

		void OnUpdate(Hazel::Timestep ts);
		void OnRender(Hazel::Camera& camera);
		void Emit(const ParticleProps& particleProps);

	private:
		struct Particle
		{
			glm::vec3 Position;
			glm::vec3 Velocity;
			glm::vec4 ColorBegin;
			glm::vec4 ColorEnd;
			glm::vec3 Rotation;
			glm::vec3 RotationVelocity;
			float SizeBegin;
			float SizeEnd;

			float LifeTime = 1.0f;
			float LifeRemaining = 0.0f;

			bool Active = false;
		};

		std::vector<Particle> m_ParticlePool;
		uint32_t m_PoolIndex = 999;

		uint32_t m_QuadVA = 0;
		Hazel::Ref<Hazel::Shader> m_ParticleShader;

		uint32_t m_ParticleShaderViewProj;
		uint32_t m_ParticleShaderTransform;
		uint32_t m_ParticleShaderColor;

	};
}
