#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace RTH
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);

		template<typename T>
		bool HasComponent()
		{
			return true;
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			RTH_CORE_ASSERT(HasComponent<T>(), "Entity already has this component!");
			return mScene->Reg().emplace<T>(mEntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			RTH_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have this component!");
			return mScene->Reg().get<T>(mEntityHandle);
		}

		template<typename T>
		T& RemoveComponent()
		{
			RTH_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have this component!");
			return mScene->Reg().remove<T>(mEntityHandle);
		}
		operator bool() const { return mEntityHandle != entt::null; }
	private:
		entt::entity mEntityHandle{entt::null};
		Scene* mScene = nullptr;
	};

}