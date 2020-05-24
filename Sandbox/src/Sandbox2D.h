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
	Hazel::Ref<Hazel::Texture2D> m_SpriteSheet;
	Hazel::Ref<Hazel::SubTexture2D> m_TextureStairs;
	Hazel::Ref<Hazel::SubTexture2D> m_TextureBarrel;
	Hazel::Ref<Hazel::SubTexture2D> m_TextureTree;

	glm::vec4 m_BackgroundColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	glm::vec4 m_QuadColor = { 0.8f, 0.4f, 0.4f, 1.0f };

	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;

	uint32_t m_MapWidth;
	uint32_t m_MapHeight;
	std::unordered_map<char, Hazel::Ref<Hazel::SubTexture2D>> s_TextureMap;
};
