#include "Rthpch.h"
#include "Application.h"
#include "Event/Event.h"
#include "Input.h"
#include <glad/glad.h>

namespace RTH
{
#define BIND_EVENT_FN(callback) std::bind(&Application::callback, this, std::placeholders::_1)
	Application* Application::sInstance = nullptr;

	static GLenum ShaderDataTypesToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}
		RTH_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	Application::Application() {
		RTH_CORE_ASSERT(!sInstance, "Application already exist");
		sInstance = this;
		mWindow = std::unique_ptr<Window>(Window::Create());
		mWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));
		mImGuiLayer = new ImGuiLayer();
		PushOverlay(mImGuiLayer);

		glGenVertexArrays(1, &mVertexArray);
		glBindVertexArray(mVertexArray);

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 0.7f, 0.7f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.4f, 0.7f, 0.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.7f, 0.7f, 0.0f, 1.0f
		};
		mVertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		
		{

			BufferLayout layout = {
				{ShaderDataType::Float3, "pos"},	
				{ ShaderDataType::Float4, "color" }
			};
			mVertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = mVertexBuffer->GetLayout();
		for (auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypesToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			index++;
		}		

		unsigned int indices[3] = { 0, 1, 2 };
		mIndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));

		std::string vertexSrc = R"(
			#version 460 core
			layout(location  = 0) in vec3 pos;
			layout(location  = 1) in vec4 color;
			out vec3 vPos;
			out vec4 vCol;
			void main ()
			{
				vPos = pos * 0.5 +0.5;
				vCol = color;
				gl_Position = vec4(pos, 1.0);
			}
		)";
		std::string fragSrc = R"(
			#version 460 core
			layout(location  = 0) out vec4 color;
			in vec3 vPos;
			in vec4 vCol;
			void main ()
			{
				//color = vec4(vPos, 1.0);
				color = vCol;
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
			glDrawElements(GL_TRIANGLES, mIndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
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