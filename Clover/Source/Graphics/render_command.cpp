#include "Clover.h"

#include <Graphics/render_command.h>
#include <opengl/opengl_renderer.h>

 
	RendererApi* RendererCommand::rendererApi_ = new OpenGLRendererApi;
