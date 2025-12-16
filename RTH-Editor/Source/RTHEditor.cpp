#include "Rthpch.h"
#include <RTH.h>
#include "Platforms/OpenGL/OpenGLShader.h"
#include "imgui.h"
#include<glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "EditorLayer.h"

namespace RTH
{
	class RTHEditor : public Engine
	{
	public:
		RTHEditor() : Engine("RTH Editor")
		{
			PushModule(new EditorLayer());
		}

		~RTHEditor()
		{

		}
	};

	Engine* CreateApplication()
	{
		return new RTHEditor();
	}
}
