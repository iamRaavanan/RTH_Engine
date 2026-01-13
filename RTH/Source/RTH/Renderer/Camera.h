#pragma once
#include<glm/glm.hpp>

namespace RTH
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) : mProjection(projection) {}
		virtual ~Camera() = default;
		const glm::mat4& GetProjection() const { return mProjection; }

	protected:
		glm::mat4 mProjection;
	};
}