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
		Camera* mainCamera = nullptr;
		glm::mat4* Camtransform = nullptr;
		auto camGroup = mRegistry.view<TransformComponent, CameraComponent>();
		for (auto entity : camGroup)
		{
			auto& [transform, camera] = camGroup.get<TransformComponent, CameraComponent>(entity);
			if (camera.primary)
			{
				mainCamera = &camera.Camera;
				Camtransform = &transform.Transform;
			}
		}
		if (mainCamera)
		{
			SpriteRenderer::BeginScene(mainCamera->GetProjection(), *Camtransform);
			auto group = mRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				SpriteRenderer::DrawQuad(transform, sprite.Color);
			}
			SpriteRenderer::EndScene();
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