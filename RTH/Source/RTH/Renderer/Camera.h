#pragma once

#include<glm/glm.hpp>

namespace RTH
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		void SetProjection(float left, float right, float bottom, float top);
		void SetPosition(const glm::vec3& position) 
		{ 
			mPosition = position;
			ReCalculateViewMatrix();
		}
		void SetRotation(float rotation) 
		{ 
			mZRotation = rotation;
			ReCalculateViewMatrix();
		}

		const glm::mat4 GetProjectionMat() const { return mProjectionMat; }
		const glm::mat4 GetViewMat() const { return mViewMat; }
		const glm::mat4 GetViewProjectionMat() const { return mViewProjectionMat; }

		const glm::vec3& GetPosition() const { return mPosition; }
		float GetRotation() const { return mZRotation; }
	private:
		glm::mat4 mProjectionMat;
		glm::mat4 mViewMat;
		glm::mat4 mViewProjectionMat;

		glm::vec3 mPosition = {0.0f, 0.0f, 0.0f};
		float mZRotation;

	private:
		void ReCalculateViewMatrix();
	};
}