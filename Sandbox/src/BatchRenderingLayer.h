#pragma once

#include "Hazel.h"


typedef unsigned int GLuint;


class BatchRenderingLayer : public Hazel::Layer
{

public:
	BatchRenderingLayer();
	virtual ~BatchRenderingLayer() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Hazel::Timestep timestep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;

private:
	Hazel::CameraController m_CameraController;

	Hazel::Ref<Hazel::Shader> m_Shader;

	GLuint m_QuadVA;
	GLuint m_QuadVB;
	GLuint m_QuadIB;

	GLuint m_Texture1;
	GLuint m_Texture2;

	float m_QuadPosition[2] = { -1.5f,  0.5f };

	glm::vec4 m_BgColor = { 0.2f, 0.0f, 0.2f, 1.0f };
	unsigned int m_FPS = 0;

	bool m_BillboardingEnabled = false;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

};
