#include "Rthpch.h"
#include "Camera.h"
#include<glm/gtc/matrix_transform.hpp>
namespace RTH
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: mProjectionMat(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), mViewMat(1.0f)
	{
		mViewProjectionMat = mProjectionMat * mViewMat;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		mProjectionMat = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		mViewProjectionMat = mProjectionMat * mViewMat;
	}

	void OrthographicCamera::ReCalculateViewMatrix()
	{
		glm::mat4 transformMat = glm::translate(glm::mat4(1.0f), mPosition) * glm::rotate(glm::mat4(1.0f), glm::radians(mZRotation), glm::vec3(0, 0, 1));
		mViewMat = glm::inverse(transformMat);
		mViewProjectionMat = mProjectionMat * mViewMat;
	}
}