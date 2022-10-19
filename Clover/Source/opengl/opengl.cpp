
#include "opengl.h"



void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
	GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         Cl_Error(message); return;
	case GL_DEBUG_SEVERITY_MEDIUM:       Cl_Warning(message); return;
	case GL_DEBUG_SEVERITY_LOW:          Cl_Warning(message); return;
	case GL_DEBUG_SEVERITY_NOTIFICATION: /*CF_TRACE(message);*/ return;
	}
	Cl_Assert(false, "");
}


