#pragma once

#include "Hazel.h"

#include "ParticleSystem.h"


class Sandbox2D : public Hazel::Layer
{

public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Hazel::Timestep timestep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;

private:
	Hazel::OrthographicCameraController m_CameraController;

	Hazel::Ref<Hazel::Texture2D> m_QuadTexture;
	Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_BackgroundColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	glm::vec4 m_QuadColor = { 0.8f, 0.4f, 0.4f, 1.0f };

	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;
};
