
#include "Clover.h"
#include "Window.h"

#include <GLFW/glfw3.h>

 
	//////////////////////////////////////////////////
	////////// INPUT /////////////////////////////////

	static WindowsWindow* current = nullptr;

	Window* Window::GetCurrentWindow()
	{
		return current;
	}

	bool WindowsInput::isKeyPressedImpl(KeyCode key)
	{
		auto window = (GLFWwindow*)(current->GetNativeWindow());

		auto state = glfwGetKey(window, key);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::isKeyReleasedImpl(KeyCode key)
	{
		auto window = (GLFWwindow*)(current->GetNativeWindow());

		auto state = glfwGetKey(window, key);

		return state == GLFW_RELEASE;
	}

	bool WindowsInput::isKeyRepeatedImpl(KeyCode key)
	{
		auto window = (GLFWwindow*)(current->GetNativeWindow());

		auto state = glfwGetKey(window, key);

		return state == GLFW_REPEAT;
	}

	bool WindowsInput::isMouseButtonPressedImpl(KeyCode key)
	{
		auto window = (GLFWwindow*)(current->GetNativeWindow());

		auto state = glfwGetMouseButton(window, key);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::isMouseButtonReleasedImpl(KeyCode key)
	{
		auto window = (GLFWwindow*)(current->GetNativeWindow());

		auto state = glfwGetMouseButton(window, key);

		return state == GLFW_RELEASE;
	}


	void WindowsInput::showMouseImpl(bool show)
	{
		auto window = (GLFWwindow*)(current->GetNativeWindow());

		if (show)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else 
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
	}

	glm::vec2 WindowsInput::mousePositionImpl()
	{
		auto window = (GLFWwindow*)(current->GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		return {(float)x, (float)y};
	}

	float WindowsInput::mouseYImpl()
	{
		return mousePositionImpl().y;
	}

	float WindowsInput::mouseXImpl()
	{
		return mousePositionImpl().x;
	}

	//////////////////////////////////////////////////
	////////// WINDOW ////////////////////////////////

	static bool glfwInitialized = false;
	static void GLFWErrorCallback(int error, const char* description)
	{
		std::stringstream ss;
		ss << "Error: " << error << " " << description;
		printf(ss.str().c_str());
	}



	WindowsWindow::WindowsWindow(const WindowProps& windowProps)
	{
		current = this;
		Init(windowProps);
	}

	WindowsWindow::~WindowsWindow()
	{
		shutdown();
	}

	void WindowsWindow::Init(const WindowProps& windowProps)
	{
		if (!glfwInitialized)
		{
			int success = glfwInit();
			glfwSetErrorCallback(GLFWErrorCallback);
			glfwInitialized = true;
		}
		int h = windowProps.height_;
		int w = windowProps.width_;
		data_.width_ = windowProps.width_;
		data_.height_ = windowProps.height_;
		data_.title = windowProps.title_;
		data_.open = true;
		window_ = glfwCreateWindow((int)data_.width_, (int)data_.height_, data_.title.c_str(), NULL, NULL);

		context_ = GraphicContext::CreateContext(window_);
		context_->Init();



		glfwSetWindowUserPointer(window_, &data_);
		setVSync(true);

		//Start OPENGL
		//glfwGetProcAddress

		//SET CALLBACKS

		glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int w, int h)
			{
				current->OnWindowResize.Broadcast(w, h);
			});

		glfwSetCharCallback(window_, [](GLFWwindow* window, unsigned int c)
			{
				current->OnKeyTyped.Broadcast(c);
			});

		glfwSetWindowCloseCallback(window_, [](GLFWwindow* window)
			{
				current->OnWindowClose.Broadcast();
			});

		glfwSetWindowFocusCallback(window_, [](GLFWwindow* window, int focus)
			{
				switch (focus)
				{
				case 1:
				{
					current->OnWindowFocus.Broadcast();
					break;
				}
				case 0:
				{
					current->OnWindowLostFocus.Broadcast();
					break;
				}
				}
			});

		glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double x, double y)
			{
				current->OnMouseMoved.Broadcast((float)x, (float)y);
			});

		glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scanCode, int action, int mods)
			{
				switch (action)
				{
				case GLFW_REPEAT:
				{
					current->OnKeyPressed.Broadcast(key, 1);
					break;
				}
				case GLFW_PRESS:
				{
					current->OnKeyPressed.Broadcast(key, 0);
					break;
				}
				case GLFW_RELEASE:
				{
					current->OnKeyReleased.Broadcast(key);
					break;
				}
				}

			});

		glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int mods)
			{
				switch (action)
				{
				case GLFW_REPEAT:
				{
					current->OnMouseButtonPressed.Broadcast(button);
					break;
				}
				case GLFW_PRESS:
				{
					current->OnMouseButtonPressed.Broadcast(button);
					break;
				}
				case GLFW_RELEASE:
				{
					current->OnMouseButtonReleased.Broadcast(button);
					break;
				}
				}

			});

		glfwSetScrollCallback(window_, [](GLFWwindow* window, double x, double y)
			{
				current->OnMouseScrolled.Broadcast(x, y);
			});

		//glViewport(0.0f, 0.0f, data_.width_, data_.height_);
		//glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();
		//glOrtho(0.0f, data_.width_, 0.0f, data_.height_, 0, 1);
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();

	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		context_->SwapBuffers();
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else {
			glfwSwapInterval(0);
		}
		data_.vSync = enabled;
	}

	bool WindowsWindow::isVSync() const
	{
		return data_.vSync;
	}

	void WindowsWindow::shutdown()
	{
		glfwDestroyWindow(window_);
		glfwTerminate();
		window_ = nullptr;
	}
