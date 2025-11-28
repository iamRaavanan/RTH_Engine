#include"Rthpch.h"
#include "Playground2D.h"
#include<RTH/Core/Entrypoint.h>
#include "imgui.h"
#include "Platforms/OpenGL/OpenGLShader.h"
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>


template<typename FN>
class Timer
{
public:
	Timer(const char* name, FN&& func)
		:mName(name), mFunc(func), mStopped(false)
	{
		mStartTimept = std::chrono::high_resolution_clock::now();
	}
	~Timer()
	{
		if (!mStopped)
			Stop();
	}
	void Stop()
	{
		auto endTimePoint = std::chrono::high_resolution_clock::now();
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(mStartTimept).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();
		mStopped = true;
		float duration = (end - start) * 0.001f;
		mFunc({ mName, duration });
	}
private:
	const char* mName;
	FN mFunc;
	bool mStopped;
	std::chrono::time_point<std::chrono::steady_clock> mStartTimept;
};

#define PROFILE_SCORE(name) Timer timer##__LINE__(name, [&](ProfileResult result) { mProfileResults.push_back(result);})

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
	PROFILE_SCORE("PG2D:OnUpdate");
	{
		PROFILE_SCORE("PG2D:OnCameraUpdate");
		mCameraController.OnUpdate(deltaTime);
	}
	RTH::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	RTH::RenderCommand::Clear();

	RTH::Renderer2D::BeginScene(mCameraController.GetCamera());
	RTH::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, mSquareColor);
	RTH::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.5f, 0.5f }, mSquareColor);
	RTH::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, mTexture);
	RTH::Renderer2D::EndScene();
}

void Playground2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(mSquareColor));
	for (auto result : mProfileResults)
	{
		char label[100];
		strcpy_s(label, "%.3fms");
		strcat_s(label, result.name);
		ImGui::Text(label, result.time);
	}
	mProfileResults.clear();
	ImGui::End();
}

void Playground2D::OnEvent(RTH::Event& e)
{
	mCameraController.OnEvent(e);
}

