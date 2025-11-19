#include "Rthpch.h"
#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLShader.h"

namespace RTH
{
	Renderer::SceneData* Renderer::mSceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}
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
	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform)
	{
		/*
		* Submit into the Render command queue.
		*/
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProj", mSceneData->mViewProjMat);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}