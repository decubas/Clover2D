
#include <Clover.h>
#include "opengl_context.h"
#include "opengl.h"
#include <GLFW/glfw3.h>

 
	OpenGLContext::OpenGLContext(GLFWwindow* glfwWindow) : windowHandle(glfwWindow)
	{
	}

	OpenGLContext::~OpenGLContext()
	{

	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(windowHandle);
		gladLoadGL();

		Cl_Info("OpenGL Info:");
		Cl_Info("	Vendor: %s", glGetString(GL_VENDOR));
		Cl_Info("	Renderer: %s", glGetString(GL_RENDERER));
		Cl_Info("	Version: %s", glGetString(GL_VERSION));

	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(windowHandle);
	}
