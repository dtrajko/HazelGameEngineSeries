#pragma once

#include "Hazel.h"


class ExampleLayer : public Hazel::Layer
{

public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Hazel::Timestep timestep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;

private:
	void DrawOpenGLTriangle();
	void UpdateInputPolling(Hazel::Timestep timestep);

private:
	Hazel::OrthographicCameraController m_CameraController;

	Hazel::ShaderLibrary m_ShaderLibrary;

	Hazel::Ref<Hazel::VertexArray> m_VertexArray;
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;

	Hazel::Ref<Hazel::Texture2D> m_Texture;
	Hazel::Ref<Hazel::Texture2D> m_ChernoLogoTexture;

	glm::vec3 m_TrianglePosition = glm::vec3(0.0f, 0.0f, 0.0f);
	float m_SquareMoveSpeed = 1.0f;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_BackgroundColor = { 0.9f, 0.8f, 0.8f, 1.0f };

	unsigned int m_FPS = 0;

};
