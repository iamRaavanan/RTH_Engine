#include "Rthpch.h"
#include "Scene.h"
#include <glm/glm.hpp>
#include "Components.h"
#include "RTH/Renderer/SpriteRenderer.h"

namespace RTH
{
	Scene::Scene()
	{

	}
	Scene::~Scene()
	{

	}

	void Scene::OnUpdate(Timestep ts)
	{

		auto group = mRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			SpriteRenderer::DrawQuad(transform, sprite.Color);
		}
	}
	entt::entity Scene::CreateEntity()
	{
		return mRegistry.create();
	}
}