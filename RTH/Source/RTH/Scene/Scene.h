#pragma once

#include "entt.hpp"
#include <RTH/Core/Timestep.h>
namespace RTH
{
	class Scene
	{
	public:
		Scene();
		~Scene();
		void OnUpdate(Timestep ts);
		entt::entity CreateEntity();
		entt::registry& Reg() { return mRegistry; }
	private:
		entt::registry mRegistry;
	};
}