#pragma once

#include "RTH/Renderer/Camera.h"

namespace RTH
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);
		void RecalculateProjection();
	private:
		float mOrthographicSize = 10.0f;
		float mOrthographicNear = -1.0f;
		float mOrthographicFar = 1.0f;
		float mAspectRatio = 0.0f;
	};
}