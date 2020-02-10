#include <hzpch.h>

#include "ParticleSystem.h"

#include "Hazel/Particles/Random.h"
#include "Hazel/Models/Cube.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>


namespace Hazel
{
	ParticleSystem::ParticleSystem()
	{
		m_ParticlePool.resize(1000);
	}

	void ParticleSystem::OnUpdate(Hazel::Timestep ts)
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.LifeRemaining -= ts;
			particle.Position += particle.Velocity * (float)ts;
			particle.Rotation += particle.Velocity * (float)ts;
		}
	}

	void ParticleSystem::OnRender(Hazel::Camera& camera)
	{
		if (!m_QuadVA)
		{
			glCreateVertexArrays(1, &m_QuadVA);
			glBindVertexArray(m_QuadVA);

			GLuint quadVB, quadIB;
			glCreateBuffers(1, &quadVB);
			glBindBuffer(GL_ARRAY_BUFFER, quadVB);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Cube::vertices), Cube::vertices, GL_STATIC_DRAW);

			glEnableVertexArrayAttrib(quadVB, 0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

			glCreateBuffers(1, &quadIB);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Cube::indices), Cube::indices, GL_STATIC_DRAW);

			m_ParticleShader = Hazel::Ref<Hazel::Shader>(Hazel::Shader::Create("assets/shaders/Particles.glsl"));
			m_ParticleShaderViewProj = glGetUniformLocation(m_ParticleShader->GetRendererID(), "u_ViewProj");
			m_ParticleShaderTransform = glGetUniformLocation(m_ParticleShader->GetRendererID(), "u_Transform");
			m_ParticleShaderColor = glGetUniformLocation(m_ParticleShader->GetRendererID(), "u_Color");
		}

		glUseProgram(m_ParticleShader->GetRendererID());
		glUniformMatrix4fv(m_ParticleShaderViewProj, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix()));

		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			// Fade away particles
			float life = particle.LifeRemaining / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
			color.a = color.a * life;

			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

			// Render
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), particle.Position)
				* glm::rotate(glm::mat4(1.0f), particle.Rotation.x, { 1.0f, 0.0f, 0.0f })
				* glm::rotate(glm::mat4(1.0f), particle.Rotation.y, { 0.0f, 1.0f, 0.0f })
				* glm::rotate(glm::mat4(1.0f), particle.Rotation.z, { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { size, size, size });
			glUniformMatrix4fv(m_ParticleShaderTransform, 1, GL_FALSE, glm::value_ptr(transform));
			glUniform4fv(m_ParticleShaderColor, 1, glm::value_ptr(color));
			glBindVertexArray(m_QuadVA);

			glDrawElements(GL_TRIANGLES, sizeof(Cube::indices), GL_UNSIGNED_INT, nullptr);

			// Hazel::Renderer::DrawCube(particle.Position, { size, size, size }, color);
			Hazel::Renderer::DrawCube(transform, color);
		}
	}

	void ParticleSystem::Emit(const ParticleProps& particleProps)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];
		particle.Active = true;
		particle.Position = particleProps.Position;
		particle.Rotation = glm::vec3(
			Random::Float() * 2.0f * glm::pi<float>(),
			Random::Float() * 2.0f * glm::pi<float>(),
			Random::Float() * 2.0f * glm::pi<float>());

		// Velocity
		particle.Velocity = particleProps.Velocity;
		particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
		particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);
		particle.Velocity.z += particleProps.VelocityVariation.z * (Random::Float() - 0.5f);

		// Color
		particle.ColorBegin = particleProps.ColorBegin;
		particle.ColorEnd = particleProps.ColorEnd;

		// Lifetime
		particle.LifeTime = particleProps.LifeTime;
		particle.LifeRemaining = particleProps.LifeTime;

		// Size
		particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
		particle.SizeEnd = particleProps.SizeEnd;

		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}
}
