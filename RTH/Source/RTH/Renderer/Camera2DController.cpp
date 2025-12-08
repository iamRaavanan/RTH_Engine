#include "Rthpch.h"
#include "Camera2DController.h"
#include "RTH/Core/Input.h"
#include "RTH/Core/KeyCodes.h"

namespace RTH
{
	Camera2DController::Camera2DController(float aspectRatio, bool rotation)
		: mAspectRatio(aspectRatio), mBounds({ -mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel }), mCamera(mBounds.Left, mBounds.Right, mBounds.Bottom, mBounds.Top), mRotation(rotation)
	{
		RTH_PROFILE_FUNCTION();
	}
	
	void Camera2DController::OnUpdate(Timestep deltaTime)
	{
		if (Input::IsKeyPressed(RTH_KEY_A))
			mCameraPos.x -= mCameraMoveSpeed * deltaTime;
		else if (RTH::Input::IsKeyPressed(RTH_KEY_D))
			mCameraPos.x += mCameraMoveSpeed * deltaTime;
		if (RTH::Input::IsKeyPressed(RTH_KEY_S))
			mCameraPos.y -= mCameraMoveSpeed * deltaTime;
		else if (RTH::Input::IsKeyPressed(RTH_KEY_W))
			mCameraPos.y += mCameraMoveSpeed * deltaTime;

		if (mRotation)
		{
			if (RTH::Input::IsKeyPressed(RTH_KEY_Q))
				mCameraRotation += mCameraRotationSpeed * deltaTime;
			if (RTH::Input::IsKeyPressed(RTH_KEY_E))
				mCameraRotation -= mCameraRotationSpeed * deltaTime;
			mCamera.SetRotation(mCameraRotation);
		}
		mCamera.SetPosition(mCameraPos);
		mCameraMoveSpeed = mZoomLevel;	
	}

	void Camera2DController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(RTH_BIND_EVENT_FN(Camera2DController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(RTH_BIND_EVENT_FN(Camera2DController::OnWindowResized));
	}

	bool Camera2DController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		mZoomLevel -= e.GetYOffset() * 0.25f;
		mZoomLevel = std::max(mZoomLevel, 0.25f);
		mBounds = { -mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel };
		mCamera.SetProjection(mBounds.Left, mBounds.Right, mBounds.Bottom, mBounds.Top);
		return false;
	}

	bool Camera2DController::OnWindowResized(WindowResizeEvent& e)
	{
		mAspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		mBounds = { -mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel };
		mCamera.SetProjection(mBounds.Left, mBounds.Right, mBounds.Bottom, mBounds.Top);
		return false;
	}
}