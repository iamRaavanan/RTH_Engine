#pragma once

#include "RTH.h"
#include "ParticleSystem.h"


enum TileType
{
	E_Water = 0,
	E_Dirt,
	E_Land
};


class Playground2D : public RTH::Module
{
public:
	Playground2D();
	virtual ~Playground2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(RTH::Timestep deltaTime) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(RTH::Event& e) override;

private:
	RTH::Camera2DController mCameraController;

	RTH::Ref<RTH::Shader> flatColorShader;
	RTH::Ref<RTH::Texture2D> mTexture;
	RTH::Ref<RTH::Texture2D> mSpriteSheet;
	//RTH::Ref<RTH::Sprite> mStairSprite, mBarrelSprite, mTreeSprite;
	RTH::Ref<RTH::VertexArray> testSquareVA;
	glm::vec4 mSquareColor = { 0.2f, 0.4f, 0.8f, 1.0f };
	glm::vec2 mPos = { 0.0f, 0.0f };

	ParticleSystem mParticleSystem;
	ParticleProps mParticle;

	uint32_t mMapWidth, mMapHeight;
	RTH::Ref<RTH::Sprite> mFailedToLoadSprite;
	std::unordered_map<TileType, RTH::Ref<RTH::Sprite>> mTextureMap;
};