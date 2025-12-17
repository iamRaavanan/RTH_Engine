#pragma once
#include "RTH/Renderer/Camera2D.h"
#include "RTH/Core/Timestep.h"
#include "RTH/Event/ApplicationEvent.h"
#include "RTH/Event/MouseEvent.h"

namespace RTH
{
	struct Camera2DBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};
	class Camera2DController
	{
	public:
		Camera2DController(float aspectRatio, bool  rotation = false);
		
		void OnUpdate(Timestep deltaTime);
		void OnEvent(Event& e);
		void OnResize(float width, float height);
		void SetZoomLevel(float value) { mZoomLevel = value; CalculateView(); }
		Camera2D& GetCamera() { return mCamera; }
		const Camera2D& GetCamera() const { return mCamera; }

		const Camera2DBounds& GetBounds() const { return mBounds; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		void CalculateView();
	private:
		float mAspectRatio;
		float mZoomLevel = 1.0f;
		Camera2DBounds mBounds;
		Camera2D mCamera;
		bool mRotation;

		glm::vec3 mCameraPos = {0.0f, 0.0f, 0.0f};
		float mCameraMoveSpeed = 2.0f;
		float mCameraRotation = 0.0f;
		float mCameraRotationSpeed = 180.0f;
	};
}