#pragma once


namespace RTH
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragSrc);
		~Shader();

		void Bind();
		void UnBind();

	private:
		uint32_t mRendererId;
	};
}
