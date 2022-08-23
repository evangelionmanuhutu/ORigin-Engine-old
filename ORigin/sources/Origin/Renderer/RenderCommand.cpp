#include "pch.h"
#include "RenderCommand.h"
#include "Platform\OpenGL\OpenGL_RendererAPI.h"

namespace Origin
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}