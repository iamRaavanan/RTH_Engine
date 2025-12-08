#include "Rthpch.h"
#include "Camera2D.h"
#include<glm/gtc/matrix_transform.hpp>
namespace RTH
{
	Camera2D::Camera2D(float left, float right, float bottom, float top)
		: mProjectionMat(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), mViewMat(1.0f)
	{
		RTH_PROFILE_FUNCTION();
		mViewProjectionMat = mProjectionMat * mViewMat;
	}

	void Camera2D::SetProjection(float left, float right, float bottom, float top)
	{
		RTH_PROFILE_FUNCTION();
		mProjectionMat = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		mViewProjectionMat = mProjectionMat * mViewMat;
	}

	void Camera2D::ReCalculateViewMatrix()
	{
		RTH_PROFILE_FUNCTION();
		glm::mat4 transformMat = glm::translate(glm::mat4(1.0f), mPosition) * glm::rotate(glm::mat4(1.0f), glm::radians(mZRotation), glm::vec3(0, 0, 1));
		mViewMat = glm::inverse(transformMat);
		mViewProjectionMat = mProjectionMat * mViewMat;
	}
}