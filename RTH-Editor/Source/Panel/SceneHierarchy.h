#pragma once
#include "RTH/Core/Core.h"
#include "RTH/Core/Log.h"
#include "RTH/Scene/Scene.h"
#include "RTH/Scene/Entity.h"
namespace RTH
{
	class SceneHierarchy
	{
	public:
		SceneHierarchy() = default;
		SceneHierarchy(const Ref<Scene>& worldContext);
		void SetWorldContext(const Ref<Scene>& context);
		void OnImGuiRender();
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> mContext;

		Entity mSelectedContext;
		friend class Scene;
	};
}