#include "hzpch.h"
#include "SubTexture2D.h"


Hazel::SubTexture2D::SubTexture2D()
{
	m_TexCoords[0] = { 0.0f, 0.0f };
	m_TexCoords[1] = { 0.0f, 0.0f };
	m_TexCoords[2] = { 0.0f, 0.0f };
	m_TexCoords[3] = { 0.0f, 0.0f };
}

Hazel::SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
	: m_Texture(texture)
{
	m_TexCoords[0] = { min.x, min.y };
	m_TexCoords[1] = { max.x, min.y };
	m_TexCoords[2] = { max.x, max.y };
	m_TexCoords[3] = { min.x, max.y };
}

Hazel::Ref<Hazel::SubTexture2D> Hazel::SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize)
{
	glm::vec2 min = { ((coords.x + 0) * cellSize.x) / texture->GetWidth(),  ((coords.y + 0) * cellSize.y) / texture->GetHeight() }; // { 0.0f, 0.0f }
	glm::vec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / texture->GetWidth(),  ((coords.y + spriteSize.y) * cellSize.y) / texture->GetHeight() }; // { 1.0f, 1.0f }
	return CreateRef<SubTexture2D>(texture, min, max);
}

Hazel::SubTexture2D::~SubTexture2D()
{
}
