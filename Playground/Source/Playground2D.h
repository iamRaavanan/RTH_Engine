#pragma once

#include "RTH.h"

class Playground2D : public RTH::Layer
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
	RTH::OrthographicCameraController mCameraController;

	RTH::Ref<RTH::Shader> flatColorShader;
	RTH::Ref<RTH::Texture2D> mTexture;
	RTH::Ref<RTH::VertexArray> testSquareVA;
	glm::vec4 mSquareColor = { 0.2f, 0.4f, 0.8f, 1.0f };
	glm::vec2 mPos = { 0.0f, 0.0f };

};