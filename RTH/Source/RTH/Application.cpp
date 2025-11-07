#include "Rthpch.h"
#include "Application.h"
#include "Event/Event.h"
#include "Input.h"
#include "RTH/Renderer/Renderer.h"

namespace RTH
{
#define BIND_EVENT_FN(callback) std::bind(&Application::callback, this, std::placeholders::_1)
	Application* Application::sInstance = nullptr;

	Application::Application() 
		: mCamera(-2.0f, 2.0f, -2.0f, 2.0f) 
	{
		RTH_CORE_ASSERT(!sInstance, "Application already exist");
		sInstance = this;
		mWindow = std::unique_ptr<Window>(Window::Create());
		mWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));
		mImGuiLayer = new ImGuiLayer();
		PushOverlay(mImGuiLayer);
		mVertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 0.7f, 0.7f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.4f, 0.7f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.7f, 0.7f, 0.0f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
				{ShaderDataType::Float3, "pos"},
				{ ShaderDataType::Float4, "color" }
		};
		vertexBuffer->SetLayout(layout);

		mVertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
		mVertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 460 core
			layout(location  = 0) in vec3 pos;
			layout(location  = 1) in vec4 color;
			uniform mat4 u_ViewProj;
			out vec3 vPos;
			out vec4 vCol;
			void main ()
			{
				vPos = pos;
				vCol = color;
				gl_Position = u_ViewProj * vec4(pos, 1.0);
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
		mShader.reset(new Shader(vertexSrc, fragSrc));
		//=============================== TEST SQUARE===============================

		testSquareVA.reset(VertexArray::Create());

		float sqVertices[3 * 4] =
		{
			-0.75f, -0.75f, 0.0f,
			0.75f, -0.75f, 0.0f,
			0.75f, 0.75f, 0.0f,
			-0.75f, 0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> testSquareVB;
		testSquareVB.reset(VertexBuffer::Create(sqVertices, sizeof(sqVertices)));
		testSquareVB->SetLayout({
				{ShaderDataType::Float3, "pos"},
			});

		testSquareVA->AddVertexBuffer(testSquareVB);

		uint32_t sqindices[6] = { 0, 1, 2, 2, 3, 0};
		std::shared_ptr<IndexBuffer> testSquareIB;
		testSquareIB.reset(IndexBuffer::Create(sqindices, sizeof(sqindices) / sizeof(uint32_t)));
		testSquareVA->SetIndexBuffer(testSquareIB);

		std::string testSquarevertexSrc = R"(
			#version 460 core
			layout(location  = 0) in vec3 pos;
			uniform mat4 u_ViewProj;
			out vec3 vPos;
			void main ()
			{
				vPos = pos;
				gl_Position = u_ViewProj * vec4(pos, 1.0);
			}
		)";
		std::string testSquarefragSrc = R"(
			#version 460 core
			layout(location  = 0) out vec4 color;
			in vec3 vPos;
			void main ()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";
		testSquareShader.reset(new Shader(testSquarevertexSrc, testSquarefragSrc));

		//=============================== TEST SQUARE===============================
	}
	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		//RTH_CORE_TRACE("{0}", e.ToString());

		for (auto it = mLayerStack.end(); it != mLayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		mLayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		mLayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run()
	{
		while (mRunning)
		{
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();
			/*mCamera.SetPosition({0.5f, 0.5f, 0.0f});
			mCamera.SetRotation(45.0f);*/
			Renderer::BeginScene(mCamera);
			Renderer::Submit(testSquareShader, testSquareVA);

			Renderer::Submit(mShader, mVertexArray);

			for (Layer* layer : mLayerStack)
			{
				layer->OnUpdate();
			}

			mImGuiLayer->Begin();
			for (Layer* layer : mLayerStack)
				layer->OnImGuiRender();
			mImGuiLayer->End();
			mWindow->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		mRunning = false;
		return true;
	}
}