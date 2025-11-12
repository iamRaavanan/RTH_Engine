#pragma once

#include<glm/glm.hpp>
namespace RTH
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragSrc);
		~Shader();

		void Bind();
		void UnBind();

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

	private:
		uint32_t mRendererId;
	};
}
