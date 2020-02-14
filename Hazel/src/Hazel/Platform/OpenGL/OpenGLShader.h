#pragma once

#include "Hazel/Renderer/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>


// TODO: REMOVE!
typedef unsigned int GLenum;


namespace Hazel
{

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		OpenGLShader(const std::string& vertexFilepath, const std::string& fragmentFilepath);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetBool(const std::string& name, bool value) override;
		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		void UploadUniformBool(const std::string& name, bool value);
		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		virtual const std::string& GetName() const override { return m_Name; };
		virtual const uint32_t GetRendererID() const override { return m_RendererID; };

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
		static std::string GetShaderTypeNameFromEnum(const GLenum shaderType);

	private:
		uint32_t m_RendererID;
		std::string m_Name;

	};

}
