#include "Rthpch.h"
#include "Scene.h"
#include <glm/glm.hpp>
#include "Components.h"
#include "RTH/Renderer/SpriteRenderer.h"
#include "Entity.h"

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
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { mRegistry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.tag = name.empty() ? "None" : name;

		return entity;
	}
}