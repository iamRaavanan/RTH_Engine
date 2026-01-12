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
		auto camView = mRegistry.view<TransformComponent, CameraComponent>();
		for (auto entity : camView)
		{
			auto& [transform, camera] = camView.get<TransformComponent, CameraComponent>(entity);
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

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		auto view = mRegistry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComp = view.get<CameraComponent>(entity);
			if (cameraComp.primary)
			{

			}
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