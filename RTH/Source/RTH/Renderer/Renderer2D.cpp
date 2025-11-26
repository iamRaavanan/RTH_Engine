#include "Rthpch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "Platforms/OpenGL/OpenGLShader.h"

namespace RTH
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> vertexArray;
		Ref<Shader> shader;
	};
	static Renderer2DStorage* sRendererData;

	void Renderer2D::Init()
	{
		sRendererData = new Renderer2DStorage();
		sRendererData->vertexArray = VertexArray::Create();

		float sqVertices[3 * 4] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Ref<VertexBuffer> testSquareVB;
		testSquareVB = VertexBuffer::Create(sqVertices, sizeof(sqVertices));
		testSquareVB->SetLayout({
				{ShaderDataType::Float3, "pos"},
			});

		sRendererData->vertexArray->AddVertexBuffer(testSquareVB);

		uint32_t sqindices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> testSquareIB;
		testSquareIB.reset(IndexBuffer::Create(sqindices, sizeof(sqindices) / sizeof(uint32_t)));
		sRendererData->vertexArray->SetIndexBuffer(testSquareIB);

		sRendererData->shader = Shader::Create("Assets/Shaders/Flatcolor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete sRendererData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<RTH::OpenGLShader>(sRendererData->shader)->Bind();
		std::dynamic_pointer_cast<RTH::OpenGLShader>(sRendererData->shader)->UploadUniformMat4("u_ViewProj", camera.GetViewProjectionMat());
		std::dynamic_pointer_cast<RTH::OpenGLShader>(sRendererData->shader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}
	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<RTH::OpenGLShader>(sRendererData->shader)->Bind();
		std::dynamic_pointer_cast<RTH::OpenGLShader>(sRendererData->shader)->UploadUniformFloat4("u_Color", color);
		sRendererData->vertexArray->Bind();
		RenderCommand::DrawIndexed(sRendererData->vertexArray);
	}
}