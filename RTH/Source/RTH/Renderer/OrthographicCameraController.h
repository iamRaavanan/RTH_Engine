#pragma once
#include "RTH/Renderer/Camera.h"
#include "RTH/Core/Timestep.h"
#include "RTH/Event/ApplicationEvent.h"
#include "RTH/Event/MouseEvent.h"

namespace RTH
{
	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool  rotation = false);
		
		void OnUpdate(Timestep deltaTime);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return mCamera; }
		const OrthographicCamera& GetCamera() const { return mCamera; }

		const OrthographicCameraBounds& GetBounds() const { return mBounds; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float mAspectRatio;
		float mZoomLevel = 1.0f;
		bool mRotation;
		OrthographicCameraBounds mBounds;
		OrthographicCamera mCamera;

		glm::vec3 mCameraPos = {0.0f, 0.0f, 0.0f};
		float mCameraMoveSpeed = 2.0f;
		float mCameraRotation = 0.0f;
		float mCameraRotationSpeed = 180.0f;
	};
}