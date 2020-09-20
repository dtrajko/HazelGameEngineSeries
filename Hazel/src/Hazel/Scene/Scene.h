#pragma once

#include "../../Hazel/Core/Timestep.h"

#include "entt.hpp"


namespace Hazel {

	class Scene {

	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		// TEMP
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep ts);

	private:
		entt::registry m_Registry;

	};


}
