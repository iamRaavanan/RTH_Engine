#pragma once

#include <glm/glm.hpp>
#include<functional>
#include "SceneCamera.h"
#include "ScriptableEntity.h"
namespace RTH
{
	struct TagComponent
	{
		std::string tag;
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string tagName) : tag(tagName) {}
	};

	struct TransformComponent
	{
		glm::mat4 Transform{1.0f};
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) : Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color { 1.0f, 1.0f, 1.0f, 1.0f};
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : Color(color) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool primary = true;
		bool isFixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;
		std::function<void()> InstantiateFunction;
		std::function<void()> DestroyFunction;
		std::function<void(ScriptableEntity*)> OnCreateFunction;
		std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunction;
		std::function<void(ScriptableEntity*)> OnDestroyFunction;

		template<typename T>
		void Bind()
		{
			InstantiateFunction = [this]() { Instance = new T();};
			DestroyFunction = [this]() {
				delete (T*)Instance;
				Instance = nullptr;
				};
			OnCreateFunction = [this](ScriptableEntity* instance) { static_cast<T*>(instance)->OnCreate();};
			OnUpdateFunction = [](ScriptableEntity* instance, Timestep ts) { static_cast<T*>(instance)->OnUpdate(ts);};
			OnDestroyFunction = [](ScriptableEntity* instance) { static_cast<T*>(instance)->OnDestroy();};
		}
	};
}