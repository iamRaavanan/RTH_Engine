#pragma once
#include "Entity.h"

namespace RTH
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}
		template<typename T>
		T& GetComponent()
		{
			return mEntity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnUpdate(Timestep deltaTime){}
		virtual void OnDestroy() {}
	private:
		Entity mEntity;
		friend class Scene;
	};
}