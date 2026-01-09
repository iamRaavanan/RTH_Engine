#include "Rthpch.h"
#include "Entity.h"

namespace RTH
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: mEntityHandle(handle), mScene(scene)
	{
	}
}