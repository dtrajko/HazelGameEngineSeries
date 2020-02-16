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

		void SetEnabled3D(bool enabled3D) { m_Enabled3D = enabled3D; };
		void SetEnabledBillboarding(bool enabledBillboarding) { m_EnabledBillboarding = enabledBillboarding; };
		void SetEnabledTexture(bool enabledTexture) { m_EnabledTexture = enabledTexture; };
		void SetTexture(Ref<Texture2D> texture) { m_Texture = texture; };
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
		uint32_t m_PoolIndex = 1499;
		bool m_Enabled3D = false;
		bool m_EnabledBillboarding = false;
		bool m_EnabledTexture = false;
		Ref<Texture2D> m_Texture;
	};
}
