#include "Rthpch.h"
#include <RTH.h>
#include "Platforms/OpenGL/OpenGLShader.h"
#include "imgui.h"
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TestLayer : public RTH::Layer
{
public:
	TestLayer() : Layer("Test"), mCamera(-2.0f, 2.0f, -2.0f, 2.0f), mCameraPos(0.0f)
	{
		mVertexArray.reset(RTH::VertexArray::Create());

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 0.7f, 0.7f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.4f, 0.7f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.7f, 0.7f, 0.0f, 1.0f
		};

		RTH::Ref<RTH::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(RTH::VertexBuffer::Create(vertices, sizeof(vertices)));
		RTH::BufferLayout layout = {
				{ RTH::ShaderDataType::Float3, "pos"},
				{ RTH::ShaderDataType::Float4, "color" }
		};
		vertexBuffer->SetLayout(layout);

		mVertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		RTH::Ref<RTH::IndexBuffer> indexBuffer;
		indexBuffer.reset(RTH::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		mVertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 460 core
			layout(location  = 0) in vec3 pos;
			layout(location  = 1) in vec4 color;
			uniform mat4 u_ViewProj;
			uniform mat4 u_Transform;
			out vec3 vPos;
			out vec4 vCol;
			void main ()
			{
				vPos = pos;
				vCol = color;
				gl_Position = u_ViewProj * u_Transform * vec4(pos, 1.0);
			}
		)";
		std::string fragSrc = R"(
			#version 460 core
			layout(location  = 0) out vec4 color;
			in vec3 vPos;
			in vec4 vCol;
			void main ()
			{
				color = vec4(vPos * 0.5 + 0.5, 1.0);
				color = vCol;
			}
		)";
		mShader.reset(RTH::Shader::Create(vertexSrc, fragSrc));
		//=============================== TEST SQUARE===============================

		testSquareVA.reset(RTH::VertexArray::Create());

		float sqVertices[5 * 4] =
		{
			-0.75f, -0.75f, 0.0f, 0.0f, 0.0f,
			 0.75f, -0.75f, 0.0f, 1.0f, 0.0f,
			 0.75f,  0.75f, 0.0f, 1.0f, 1.0f,
			-0.75f,  0.75f, 0.0f, 0.0f, 1.0f
		};

		RTH::Ref<RTH::VertexBuffer> testSquareVB;
		testSquareVB.reset(RTH::VertexBuffer::Create(sqVertices, sizeof(sqVertices)));
		testSquareVB->SetLayout({
				{RTH::ShaderDataType::Float3, "pos"},
				{RTH::ShaderDataType::Float2, "texCoord"}
			});

		testSquareVA->AddVertexBuffer(testSquareVB);

		uint32_t sqindices[6] = { 0, 1, 2, 2, 3, 0 };
		RTH::Ref<RTH::IndexBuffer> testSquareIB;
		testSquareIB.reset(RTH::IndexBuffer::Create(sqindices, sizeof(sqindices) / sizeof(uint32_t)));
		testSquareVA->SetIndexBuffer(testSquareIB);

		std::string flatColorvertexSrc = R"(
			#version 460 core
			layout(location  = 0) in vec3 pos;
			uniform mat4 u_ViewProj;
			uniform mat4 u_Transform;
			out vec3 vPos;
			void main ()
			{
				vPos = pos;
				gl_Position = u_ViewProj * u_Transform * vec4(pos, 1.0);
			}
		)";
		std::string flatColorfragSrc = R"(
			#version 460 core
			layout(location  = 0) out vec4 color;
			in vec3 vPos;
			uniform vec3 u_Color;
			void main ()
			{
				color = vec4(u_Color, 1.0f);
			}
		)";
		flatColorShader.reset(RTH::Shader::Create(flatColorvertexSrc, flatColorfragSrc));

		textureShader.reset(RTH::Shader::Create("Assets/Shaders/Texture.glsl"));
		//=============================== TEST SQUARE===============================

		texture = RTH::Texture2D::Create("Assets/Textures/Checkerboard.png");
		iconTexture = RTH::Texture2D::Create("Assets/Textures/ChernoLogo.png");
		std::dynamic_pointer_cast<RTH::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<RTH::OpenGLShader>(textureShader)->UploadUniformInt("u_Tex", 0);
	}

	void OnUpdate(RTH::Timestep deltaTime) override
	{
		RTH_CORE_TRACE("DeltaTime: {0}s : {1}ms", deltaTime.GetSeconds(), deltaTime.GetMilliSeconds());
		if (RTH::Input::IsKeyPressed(RTH_KEY_LEFT))
			mCameraPos.x -= mCameraMoveSpeed * deltaTime;
		else if (RTH::Input::IsKeyPressed(RTH_KEY_RIGHT))
			mCameraPos.x += mCameraMoveSpeed * deltaTime;
		if (RTH::Input::IsKeyPressed(RTH_KEY_DOWN))
			mCameraPos.y -= mCameraMoveSpeed * deltaTime;
		else if (RTH::Input::IsKeyPressed(RTH_KEY_UP))
			mCameraPos.y += mCameraMoveSpeed * deltaTime;

		if (RTH::Input::IsKeyPressed(RTH_KEY_A))
			mCameraRotation += mCameraRotationSpeed * deltaTime;
		if (RTH::Input::IsKeyPressed(RTH_KEY_D))
			mCameraRotation -= mCameraRotationSpeed * deltaTime;

		RTH::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RTH::RenderCommand::Clear();
		mCamera.SetPosition(mCameraPos);
		mCamera.SetRotation(mCameraRotation);
		RTH::Renderer::BeginScene(mCamera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<RTH::OpenGLShader>(flatColorShader)->Bind();
		std::dynamic_pointer_cast<RTH::OpenGLShader>(flatColorShader)->UploadUniformFloat3("u_Color", mSquareColor);
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				glm::vec3 pos(-1.5f + i * 0.16f, -1.5f + j * 0.16f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				RTH::Renderer::Submit(flatColorShader, testSquareVA, transform);
			}
		}
		texture->Bind();
		RTH::Renderer::Submit(textureShader, testSquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		iconTexture->Bind();
		RTH::Renderer::Submit(textureShader, testSquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		//RTH::Renderer::Submit(mShader, mVertexArray);
		RTH::Renderer::EndScene();

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(mSquareColor));
		ImGui::End();
	}
	void OnEvent(RTH::Event& evnt) override
	{
		//RTH::EventDispatcher dispatcher(evnt);
		//dispatcher.Dispatch<RTH::KeyPressedEvent>(RTH_BIND_EVENT_FN(TestLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(RTH::KeyPressedEvent& event)
	{
		/*if (event.GetKeyCode() == RTH_KEY_LEFT)
		{
			mCameraPos.x -= mCameraSpeed;
		}
		else if (event.GetKeyCode() == RTH_KEY_RIGHT)
		{
			mCameraPos.x += mCameraSpeed;
		}
		else if (event.GetKeyCode() == RTH_KEY_DOWN)
		{
			mCameraPos.y -= mCameraSpeed;
		}
		else if (event.GetKeyCode() == RTH_KEY_UP)
		{
			mCameraPos.y += mCameraSpeed;
		}
		return false;*/
	}
private:

	RTH::Ref<RTH::Shader> mShader;
	RTH::Ref<RTH::VertexArray> mVertexArray;
	// Test
	RTH::Ref<RTH::Shader> flatColorShader;
	RTH::Ref<RTH::VertexArray> testSquareVA;

	RTH::Ref<RTH::Shader> textureShader;
	RTH::Ref<RTH::Texture2D> texture, iconTexture;

	RTH::OrthographicCamera mCamera;
	glm::vec3 mCameraPos;
	float mCameraMoveSpeed = 2.0f;
	float mCameraRotation = 0.0f;
	float mCameraRotationSpeed = 180.0f;
	glm::vec3 mSquareColor = { 0.2f, 0.4f, 0.8f };
};

class PlaygroundApp : public RTH::Application
{
public:
	PlaygroundApp()
	{
		PushLayer(new TestLayer());
	}

	~PlaygroundApp()
	{

	}
};

RTH::Application* RTH::CreateApplication()
{
	return new PlaygroundApp();
}