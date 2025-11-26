#include"Rthpch.h"
#include "Playground2D.h"
#include<RTH/Core/Entrypoint.h>
#include "imgui.h"
#include "Platforms/OpenGL/OpenGLShader.h"
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Playground2D::Playground2D()
	:Layer("2DPlayground"), mCameraController(1280.0f / 720.0f)
{
	testSquareVA = RTH::VertexArray::Create();

	float sqVertices[3 * 4] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	RTH::Ref<RTH::VertexBuffer> testSquareVB;
	testSquareVB = RTH::VertexBuffer::Create(sqVertices, sizeof(sqVertices));
	testSquareVB->SetLayout({
			{RTH::ShaderDataType::Float3, "pos"},
		});

	testSquareVA->AddVertexBuffer(testSquareVB);

	uint32_t sqindices[6] = { 0, 1, 2, 2, 3, 0 };
	RTH::Ref<RTH::IndexBuffer> testSquareIB;
	testSquareIB.reset(RTH::IndexBuffer::Create(sqindices, sizeof(sqindices) / sizeof(uint32_t)));
	testSquareVA->SetIndexBuffer(testSquareIB);

	flatColorShader = RTH::Shader::Create("Assets/Shaders/Flatcolor.glsl");
}

void Playground2D::OnAttach()
{
	
}

void Playground2D::OnDetach()
{

}

void Playground2D::OnUpdate(RTH::Timestep deltaTime)
{
	mCameraController.OnUpdate(deltaTime);
	RTH::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	RTH::RenderCommand::Clear();

	RTH::Renderer2D::BeginScene(mCameraController.GetCamera());
	/*std::dynamic_pointer_cast<RTH::OpenGLShader>(flatColorShader)->Bind();
	std::dynamic_pointer_cast<RTH::OpenGLShader>(flatColorShader)->UploadUniformFloat4("u_Color", mSquareColor);*/
	RTH::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.9f, 0.3f, 0.3f, 1.0f });
	RTH::Renderer2D::EndScene();
}

void Playground2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(mSquareColor));
	ImGui::End();
}

void Playground2D::OnEvent(RTH::Event& e)
{
	mCameraController.OnEvent(e);
}

