#include "hzpch.h"

#include "OpenGLFramebuffer.h"

namespace Hazel {

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
	}

	void OpenGLFramebuffer::Invalidate()
	{
	}

	void OpenGLFramebuffer::Bind()
	{
	}

	void OpenGLFramebuffer::Unbind()
	{
	}

	const FramebufferSpecification& OpenGLFramebuffer::GetSpecification() const
	{
		return m_Specification;
	}
}
