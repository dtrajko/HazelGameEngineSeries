#pragma once

#include "Hazel.h"


class OBJLayer : public Hazel::Layer
{

public:
	OBJLayer();
	virtual ~OBJLayer() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Hazel::Timestep timestep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;

private:
	Hazel::CameraController m_CameraController;

	Hazel::Ref<Hazel::VertexArray> m_ModelVA;
	Hazel::Ref<Hazel::Shader> m_ModelShader;
	Hazel::Ref<Hazel::Texture2D> m_ModelTexture;
	glm::vec3 m_ModelRotation = { 0.0f, 0.0f, 0.0f };

	glm::vec4 m_BackgroundColor = { 0.1f, 0.1f, 0.2f, 1.0f };
	unsigned int m_FPS = 0;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

};
