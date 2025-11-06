#include "Rthpch.h"
#include "Renderer.h"

namespace RTH
{
	void Renderer::BeginScene()
	{
		/*
		* Takes all scene param, shader we are using gets proper uniform
		* Camera details like projection and view matrix
		*/
	}
	void Renderer::EndScene()
	{
		/*
		* 
		*/
	}
	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		/*
		* Submit into the Render command queue.
		*/
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}