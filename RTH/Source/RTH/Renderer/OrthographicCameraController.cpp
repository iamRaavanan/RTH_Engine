#include "Rthpch.h"
#include "OrthographicCameraController.h"
#include "RTH/Core/Input.h"
#include "RTH/Core/KeyCodes.h"

namespace RTH
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: mAspectRatio(aspectRatio), mBounds({ -mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel }), mCamera(mBounds.Left, mBounds.Right, mBounds.Bottom, mBounds.Top), mRotation(rotation)
	{
		RTH_PROFILE_FUNCTION();
	}
	
	void OrthographicCameraController::OnUpdate(Timestep deltaTime)
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

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(RTH_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(RTH_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		mZoomLevel -= e.GetYOffset() * 0.25f;
		mZoomLevel = std::max(mZoomLevel, 0.25f);
		mBounds = { -mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel };
		mCamera.SetProjection(mBounds.Left, mBounds.Right, mBounds.Bottom, mBounds.Top);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		mAspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		mBounds = { -mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel };
		mCamera.SetProjection(mBounds.Left, mBounds.Right, mBounds.Bottom, mBounds.Top);
		return false;
	}
}