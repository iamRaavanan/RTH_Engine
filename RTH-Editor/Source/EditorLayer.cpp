#include"Rthpch.h"
#include "EditorLayer.h"
#include<RTH/Core/Entrypoint.h>
#include "imgui.h"
#include "Platforms/OpenGL/OpenGLShader.h"
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace RTH
{
	EditorLayer::EditorLayer()
		:Module("2DPlayground"), mCameraController(1280.0f / 720.0f)
	{
		mTexture = Texture2D::Create("Assets/Textures/Checkerboard.png");

		mCameraController.SetZoomLevel(2.0f);

		FrameBufferSpecification Spec;
		Spec.Width = 1280;
		Spec.Height = 720;
		mFrameBuffer = FrameBuffer::Create(Spec);
	}

	void EditorLayer::OnAttach()
	{
		// Init here

	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep deltaTime)
	{
		RTH_PROFILE_FUNCTION();

		mCameraController.OnUpdate(deltaTime);
		SpriteRenderer::ResetStats();
		{
			RTH_PROFILE_SCOPE("Render Preparation");
			mFrameBuffer->Bind();
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();
		}
#if 1
		{
			float ts = deltaTime;
			static float rotation = 0.0f;
			rotation += 0.25f;
			RTH_PROFILE_SCOPE("Render Draw");
			SpriteRenderer::BeginScene(mCameraController.GetCamera());
			SpriteRenderer::DrawRotatedQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(-rotation), mSquareColor);
			SpriteRenderer::DrawQuad({ -1.0f, 0.0f }, { 0.5f, 0.5f }, mSquareColor);
			SpriteRenderer::DrawQuad({ 1.0f, 0.0f }, { 0.5f, 0.5f }, { 0.5f, 0.8f, 0.2f, 1.0f });
			SpriteRenderer::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, mTexture/*, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f)*/, 10.0f);
			SpriteRenderer::DrawRotatedQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), mTexture, glm::vec4(1.0f, 0.9f, 0.9f, 1.0f), 10.0f);
			//SpriteRenderer::EndScene();

			//SpriteRenderer::BeginScene(mCameraController.GetCamera());
			for (float y = -10.0f; y < 10.0f; y += 0.1f)
			{
				for (float x = -10.0f; x < 10.0f; x += 0.1f)
				{
					glm::vec4 color = { (x + 10.0f) / 15.0f, 0.4f, (y + 10.0f) / 15.0f, 0.65f };
					SpriteRenderer::DrawQuad({ x, y, 0.0f }, { 0.45f, 0.45f }, color);
				}
			}
			SpriteRenderer::EndScene();
		}
#endif

		mFrameBuffer->UnBind();
	}

	void EditorLayer::OnImGuiRender()
	{
		RTH_PROFILE_FUNCTION();
		static bool dockingEnable = true;
		if (dockingEnable)
		{
			static bool open_dockspace = true;
			static bool opt_fullscreen = true;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->WorkPos);
				ImGui::SetNextWindowSize(viewport->WorkSize);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
			// and handle the pass-thru hole, so we ask Begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", &open_dockspace, window_flags);
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// Submit the DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Exit"))
						Engine::GetApplication().Close();
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImGui::Begin("Settings");
			auto stats = SpriteRenderer::GetStats();
			ImGui::Text("SpriteRenderer Stas");
			ImGui::Text("Draw Calls	: %d", stats.DrawCalls);
			ImGui::Text("Quads		: %d", stats.QuadCount);
			ImGui::Text("Vertices	: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices	: %d", stats.GetTotalIndexCount());
			ImGui::ColorEdit4("Square Color", glm::value_ptr(mSquareColor));

			uint32_t textureID = mFrameBuffer->GetColorAttachmentRendererID(); //mSpriteSheet->GetRendererId();
			ImGui::Image((void*)textureID, ImVec2(1280, 720));
			ImGui::End();

			ImGui::End();
		}
		else
		{

			ImGui::Begin("Settings");
			auto stats = SpriteRenderer::GetStats();
			ImGui::Text("SpriteRenderer Stas");
			ImGui::Text("Draw Calls	: %d", stats.DrawCalls);
			ImGui::Text("Quads		: %d", stats.QuadCount);
			ImGui::Text("Vertices	: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices	: %d", stats.GetTotalIndexCount());
			ImGui::ColorEdit4("Square Color", glm::value_ptr(mSquareColor));

			uint32_t textureID = mFrameBuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2(1280, 720));
			ImGui::End();

		}
	}

	void EditorLayer::OnEvent(Event& e)
	{
		mCameraController.OnEvent(e);
	}


}