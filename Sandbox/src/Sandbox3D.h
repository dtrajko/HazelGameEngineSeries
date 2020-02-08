#pragma once

#include "Hazel.h"


class Sandbox3D : public Hazel::Layer
{

public:
	Sandbox3D();
	virtual ~Sandbox3D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Hazel::Timestep timestep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;

private:
	Hazel::CameraController m_CameraController;

	Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::Texture2D> m_Texture;
	Hazel::Ref<Hazel::Shader> m_FlatColorShader;

	Hazel::Ref<Hazel::VertexArray> m_ChaletVA;
	Hazel::Ref<Hazel::Texture2D> m_ChaletTexture;
	Hazel::Ref<Hazel::Shader> m_ChaletShader;

	glm::vec4 m_BackgroundColor = { 1.0f, 1.0f, 0.8f, 1.0f };
	glm::vec4 m_SquareColor1 = { 1.0f, 0.6f, 0.6f, 0.5f };
	glm::vec4 m_SquareColor2 = { 0.6f, 1.0f, 0.6f, 0.5f };
	glm::vec4 m_SquareColor3 = { 0.6f, 0.6f, 1.0f, 0.5f };

};
