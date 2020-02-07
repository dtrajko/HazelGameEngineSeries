#pragma once

#include "Hazel.h"


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

	// Temporary
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::Shader> m_FlatColorShader;

	glm::vec4 m_BackgroundColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

};
