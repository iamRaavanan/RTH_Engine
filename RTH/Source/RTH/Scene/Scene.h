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
		Entity CreateEntity(const std::string& name = std::string());
		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		entt::registry& Reg() { return mRegistry; }
	private:
		entt::registry mRegistry;
		uint32_t mViewportWidth = 0;
		uint32_t mViewportHeight = 0;
		friend class Entity;
	};
}