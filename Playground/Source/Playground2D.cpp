#include"Rthpch.h"
#include "Playground2D.h"
#include<RTH/Core/Entrypoint.h>
#include "imgui.h"
#include "Platforms/OpenGL/OpenGLShader.h"
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const char* sMapTiles = R"(000000000000000000000000000000000000000000000000
000000000000000000000000000000000000000000000000
000000011111111111000000000000000000111111110000
000000112222222222110000000000000001222222221100
000001122222222222221000000000000012222222222210
000011222222222222222110000000000112222222222221
000112222222211112222221100000001122222221112222
000122222221100001222222210000002222222110001222
000122222211000000122222221000012222222100000222
000122222110000000012222222100122222221000000122
000122221100000000001222222210222222210000000112
000122221000000000000122222222222222210000000112
000112221000000000000112222222222222110000000112
000011222100000000000012222222222221100000001122
000001222210000000000001222222222221000000001110
000000122221000000000001122222222211000000011100
000000012222100000000000112222222211000000111000
000000001222210000000000012222222210000001110000
000000000122221000000000001222222210000011100000
000000000012222100000000000122222210000111000000
000000000001222210000022000012222221001110000000
000000000000122221000222200001222222011100000000
000000000000012222100222220000122222111000000000
000000000000001222212222222000012222210000000000
000000000000000122222222222200001222210000000000
000000000000000012222222222200000122221000000000
000000000000000122222222222222000012222100000000
000000000000001222222222222222100001222210000000
000000000000012222222222222222210000122222100000
000000000000122222222222222222221000012222210000
000000000001222222211111122222222210001222221000
000000000012222221100000011222222221000122222100
000000000122222210000000000122222222100012222210
000000001222222100000000000012222222210001222221
000000012222221000000000000001222222221000122222
000000122222210000000000000000122222221100122222
000001222222100000000000000000012222222210012222
000012222222000000000000000000001222222221001222
000122222221000000000000000000000222222222001222
001222222221000000000000000000000122222222101222
002222222221100000000000000000000112222222122222
000222222222110000000000000000000011222222222220
000022222222211000000000000000000001222222222200
000002222222222110000000000000000000122222222200
000000111111111111000000000000000000111111111100
000000000000000000000000000000000000000000000000)";


Playground2D::Playground2D()
	:Module("2DPlayground"), mCameraController(1280.0f / 720.0f)
{	
	mTexture = RTH::Texture2D::Create("Assets/Textures/Checkerboard.png");
	mSpriteSheet = RTH::Texture2D::Create("Assets/Textures/RPG_spritesheet.png");

	mTextureMap[TileType::E_Water] = RTH::Sprite::CreateFromCoords(mSpriteSheet, {11, 11}, {128, 128});
	mTextureMap[TileType::E_Dirt] = RTH::Sprite::CreateFromCoords(mSpriteSheet, { 6, 11 }, { 128, 128 });
	mTextureMap[TileType::E_Land] = RTH::Sprite::CreateFromCoords(mSpriteSheet, { 1, 11 }, { 128, 128 });
	mFailedToLoadSprite = RTH::Sprite::CreateFromCoords(mSpriteSheet, { 8, 2 }, { 128, 128 });

	mParticle.ColorBegin = { 154 / 255.0f, 40 / 255.0f, 123 / 255.0f, 1.0f };
	mParticle.ColorEnd = { 100 / 255.0f, 19 / 255.0f, 198 / 255.0f, 1.0f };
	mParticle.SizeBegin = 0.5f, mParticle.SizeVariation = 0.3f, mParticle.SizeEnd = 0.0f;
	mParticle.LifeTime = 1.0f;
	mParticle.Velocity = { 0.0f, 0.0f };
	mParticle.VelocityVariation = { 3.0f, 1.0f };
	mParticle.Position = { 0.0f, 0.0f };
	mMapWidth = mMapHeight = 48;
	mCameraController.SetZoomLevel(2.0f);
}

void Playground2D::OnAttach()
{
	// Init here
	
}

void Playground2D::OnDetach()
{

}

void Playground2D::OnUpdate(RTH::Timestep deltaTime)
{
	RTH_PROFILE_FUNCTION();
	
	mCameraController.OnUpdate(deltaTime);
	RTH::SpriteRenderer::ResetStats();
	{
		RTH_PROFILE_SCOPE("Render Preparation");
		RTH::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RTH::RenderCommand::Clear();
	}
#if 0
	{
		float ts = deltaTime;
		static float rotation = 0.0f;
		rotation += 0.25f;
		RTH_PROFILE_SCOPE("Render Draw");
		RTH::SpriteRenderer::BeginScene(mCameraController.GetCamera());
		RTH::SpriteRenderer::DrawRotatedQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(-rotation), mSquareColor);
		RTH::SpriteRenderer::DrawQuad({ -1.0f, 0.0f }, { 0.5f, 0.5f }, mSquareColor);
		RTH::SpriteRenderer::DrawQuad({ 1.0f, 0.0f }, { 0.5f, 0.5f }, {0.5f, 0.8f, 0.2f, 1.0f});
		RTH::SpriteRenderer::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, mTexture/*, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f)*/, 10.0f);
		RTH::SpriteRenderer::DrawRotatedQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), mTexture, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f), 10.0f);
		//RTH::SpriteRenderer::EndScene();

		//RTH::SpriteRenderer::BeginScene(mCameraController.GetCamera());
		for (float y = -10.0f; y < 10.0f; y += 0.1f)
		{
			for (float x = -10.0f; x < 10.0f; x += 0.1f)
			{
				glm::vec4 color = { (x + 10.0f) / 15.0f, 0.4f, (y + 10.0f) / 15.0f, 0.65f };
				RTH::SpriteRenderer::DrawQuad({ x, y, 0.0f }, { 0.45f, 0.45f }, color);
			}
		}
		RTH::SpriteRenderer::EndScene();
	}
#endif

	if (RTH::Input::IsMouseButtonPressed(RTH_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = RTH::Input::GetMousePosition();
		auto width = RTH::Engine::GetApplication().GetWindow().GetWidth();
		auto height = RTH::Engine::GetApplication().GetWindow().GetHeight();

		auto bounds = mCameraController.GetBounds();
		auto pos = mCameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		mParticle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			mParticleSystem.Emit(mParticle);
	}

	mParticleSystem.OnUpdate(deltaTime);
	mParticleSystem.OnRender(mCameraController.GetCamera());

	RTH::SpriteRenderer::BeginScene(mCameraController.GetCamera());

	for (uint32_t x = 0; x < mMapHeight; x++)
	{
		for (uint32_t y = 0; y < mMapWidth; y++)
		{
			char value = sMapTiles[x + y * (mMapWidth+1)];
			if (value == '\n' || value == '\r')
				continue;
			TileType type = static_cast<TileType>(value - '0');
			RTH::Ref<RTH::Sprite> sprite;
			if (mTextureMap.find(type) != mTextureMap.end())
			{
				sprite = mTextureMap[type];
			}
			else
			{
				sprite = mFailedToLoadSprite;
			}
			RTH::SpriteRenderer::DrawSprite({ x- mMapWidth/2, y - mMapHeight/2, 0.3f }, { 1.0f, 1.0f }, sprite);
		}
	}
	RTH::SpriteRenderer::EndScene();
}

void Playground2D::OnImGuiRender()
{
	RTH_PROFILE_FUNCTION();
	ImGui::Begin("Settings");
	auto stats = RTH::SpriteRenderer::GetStats();
	ImGui::Text("SpriteRenderer Stas");
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

