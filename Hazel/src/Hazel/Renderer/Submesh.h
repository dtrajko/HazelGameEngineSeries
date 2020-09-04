#pragma once

#include "Hazel/Core/Timestep.h"

#include <glm/glm.hpp>


namespace Hazel {

	class Submesh
	{
	public:
		glm::mat4 Transform;
		int IndexCount;
		int BaseIndex;
		int BaseVertex;

	};

}
