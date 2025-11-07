#include "Rthpch.h"
#include "Renderer.h"

namespace RTH
{
	Renderer::SceneData* Renderer::mSceneData = new Renderer::SceneData;
	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		/*
		* Takes all scene param, shader we are using gets proper uniform
		* Camera details like projection and view matrix
		*/
		mSceneData->mViewProjMat = camera.GetViewProjectionMat();
	}
	void Renderer::EndScene()
	{
		/*
		* 
		*/
	}
	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		/*
		* Submit into the Render command queue.
		*/
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProj", mSceneData->mViewProjMat);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}