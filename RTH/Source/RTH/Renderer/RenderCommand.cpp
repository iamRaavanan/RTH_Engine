#include "Rthpch.h"
#include "RenderCommand.h"
#include "Platforms/OpenGL/OpenGLRendererAPI.h"

namespace RTH
{
	RendererAPI* RenderCommand::sRendererAPI = new OpenGLRendererAPI;
}