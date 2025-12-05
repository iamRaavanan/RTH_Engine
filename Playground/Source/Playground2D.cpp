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
	mTexture = RTH::Texture2D::Create("Assets/Textures/Checkerboard.png");
}

void Playground2D::OnAttach()
{
	
}

void Playground2D::OnDetach()
{

}

void Playground2D::OnUpdate(RTH::Timestep deltaTime)
{
	RTH_PROFILE_FUNCTION();
	
	mCameraController.OnUpdate(deltaTime);
	RTH::Renderer2D::ResetStats();
	{
		RTH_PROFILE_SCOPE("Render Preparation");
		RTH::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RTH::RenderCommand::Clear();
	}

	{
		float ts = deltaTime;
		static float rotation = 0.0f;
		rotation += 0.25f;
		RTH_PROFILE_SCOPE("Render Draw");
		RTH::Renderer2D::BeginScene(mCameraController.GetCamera());
		RTH::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, -rotation, mSquareColor);
		RTH::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.5f, 0.5f }, mSquareColor);
		RTH::Renderer2D::DrawQuad({ 1.0f, 0.0f }, { 0.5f, 0.5f }, {0.5f, 0.8f, 0.2f, 1.0f});
		RTH::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, mTexture/*, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f)*/, 10.0f);
		RTH::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, mTexture, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f), 10.0f);
		//RTH::Renderer2D::EndScene();

		//RTH::Renderer2D::BeginScene(mCameraController.GetCamera());
		for (float y = -10.0f; y < 10.0f; y += 0.1f)
		{
			for (float x = -10.0f; x < 10.0f; x += 0.1f)
			{
				glm::vec4 color = { (x + 10.0f) / 15.0f, 0.4f, (y + 10.0f) / 15.0f, 0.65f };
				RTH::Renderer2D::DrawQuad({ x, y, 0.0f }, { 0.45f, 0.45f }, color);
			}
		}
		RTH::Renderer2D::EndScene();
	}
}

void Playground2D::OnImGuiRender()
{
	RTH_PROFILE_FUNCTION();
	ImGui::Begin("Settings");
	auto stats = RTH::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stas");
	ImGui::Text("Draw Calls	: %d", stats.DrawCalls);
	ImGui::Text("Quads		: %d", stats.QuadCount);
	ImGui::Text("Vertices	: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices	: %d", stats.GetTotalIndexCount());
	ImGui::ColorEdit4("Square Color", glm::value_ptr(mSquareColor));
	ImGui::End();
}

void Playground2D::OnEvent(RTH::Event& e)
{
	mCameraController.OnEvent(e);
}

