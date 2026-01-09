#pragma once

#include "entt.hpp"
#include <RTH/Core/Timestep.h>
namespace RTH
{
	class Entity;
	class Scene
	{
	public:
		Scene();
		~Scene();
		void OnUpdate(Timestep ts);
		Entity CreateEntity(const std::string& name = std::string());
		entt::registry& Reg() { return mRegistry; }
	private:
		entt::registry mRegistry;
	};
}