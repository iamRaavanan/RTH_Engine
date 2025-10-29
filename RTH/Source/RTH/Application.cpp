#include "Rthpch.h"
#include "Application.h"
#include "Event/Event.h"
#include "Input.h"
#include <glad/glad.h>

namespace RTH
{
#define BIND_EVENT_FN(callback) std::bind(&Application::callback, this, std::placeholders::_1)
	Application* Application::sInstance = nullptr;
	Application::Application() {
		RTH_CORE_ASSERT(!sInstance, "Application already exist");
		sInstance = this;
		mWindow = std::unique_ptr<Window>(Window::Create());
		mWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));
		mImGuiLayer = new ImGuiLayer();
		PushOverlay(mImGuiLayer);

		glGenVertexArrays(1, &mVertexArray);
		glBindVertexArray(mVertexArray);

		glGenBuffers(1, &mVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);

		float vertices[9] =
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &mIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		std::string vertexSrc = R"(
			#version 460 core
			layout(location  = 0) in vec3 pos;
			out vec3 vPos;
			void main ()
			{
				vPos = pos * 0.5 +0.5;
				gl_Position = vec4(pos, 1.0);
			}
		)";
		std::string fragSrc = R"(
			#version 460 core
			layout(location  = 0) out vec4 color;
			in vec3 vPos;
			void main ()
			{
				color = vec4(vPos, 1.0);
			}
		)";
		mShader.reset(new Shader(vertexSrc, fragSrc));
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
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			mShader->Bind();
			glBindVertexArray(mVertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
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