#ifndef _OPENGL_CONTEXT_H_
#define _OPENGL_CONTEXT_H_ 1


#include "Graphics/graphiccontext.h"


struct GLFWwindow;


    // Initialize OpenGL & GLFW context
    class OpenGLContext : public GraphicContext {
    public:
        OpenGLContext(GLFWwindow* glfwWindow);
        ~OpenGLContext();

        virtual void Init() override;
        virtual void SwapBuffers() override;

    protected:
        GLFWwindow* windowHandle;
    };

#endif