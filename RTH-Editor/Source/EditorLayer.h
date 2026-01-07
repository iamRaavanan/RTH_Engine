#pragma once

#include "RTH.h"

namespace RTH
{
	class EditorLayer : public Module
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep deltaTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

	private:
		Camera2DController mCameraController;

		Ref<Shader> flatColorShader;
		Ref<Texture2D> mTexture;
		Ref<FrameBuffer> mFrameBuffer;
		Ref<VertexArray> testSquareVA;
		Ref<Scene> ActiveScene;

		entt::entity squareEntity;
		bool mViewportFocused = false;
		bool mViewportHovered = false;
		glm::vec2 mViewportSize= {0.0f, 0.0f };
		glm::vec4 mSquareColor = { 0.2f, 0.4f, 0.8f, 1.0f };
	};
}
