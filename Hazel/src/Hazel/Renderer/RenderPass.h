#pragma once

#include "Framebuffer.h"


namespace Hazel {

	struct RenderPassSpecification
	{
		Ref<Framebuffer> TargetFramebuffer;
	};

	class RenderPass
	{
	public:
		static Ref<RenderPass> Create(RenderPassSpecification geoRenderPassSpec);
	};

}
