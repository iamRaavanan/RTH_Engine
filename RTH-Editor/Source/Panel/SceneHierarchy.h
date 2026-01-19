#pragma once
#include "RTH/Core/Core.h"
#include "RTH/Core/Log.h"
#include "RTH/Scene/Scene.h"

namespace RTH
{
	class SceneHierarchy
	{
	public:
		SceneHierarchy() = default;
		SceneHierarchy(const Ref<Scene>& worldContext);
		void SetWorldContext(const Ref<Scene>& context);
		void OnImGuiRender();
	private:
		Ref<Scene> mContext;
		friend class Scene;
	};
}