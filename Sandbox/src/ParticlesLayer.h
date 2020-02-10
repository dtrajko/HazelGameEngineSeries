#pragma once

#include "Hazel.h"

#include "Hazel/Particles/ParticleSystem.h"


class ParticlesLayer : public Hazel::Layer
{

public:
	ParticlesLayer();
	virtual ~ParticlesLayer() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Hazel::Timestep timestep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;

private:
	Hazel::CameraController m_CameraController;

	Hazel::ParticleProps m_Particle;
	Hazel::ParticleSystem m_ParticleSystem;
	glm::vec4 m_BackgroundColor = { 0.1f, 0.1f, 0.2f, 1.0f };
	float m_Velocity = 3.0f;
	float m_RotationVelocity = 0.0f;
	unsigned int m_FPS = 0;
	bool m_Enabled3D = true;

};
