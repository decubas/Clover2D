#pragma once

#ifndef _BASE_WINDOW_H_
#define _BASE_WINDOW_H_ 1

#include "Clover.h"
#include "Delegate/Delegate.h"
#include "graphiccontext.h"

#define CloverInput Input


struct GLFWwindow; 

	//Singleton Input
	class Input
	{
	public:



		inline static bool isKeyPressed(KeyCode key) { return instance->isKeyPressedImpl(key); };
		inline static bool isKeyReleased(KeyCode key) { return instance->isKeyReleasedImpl(key); };
		inline static bool isKeyRepeated(KeyCode key) { return instance->isKeyRepeatedImpl(key); };
		 		 
		inline static  bool isMouseButtonPressed(KeyCode key) { return instance->isMouseButtonPressedImpl(key); };
		inline static  bool isMouseButtonReleased(KeyCode key) { return instance->isMouseButtonReleasedImpl(key); };
		 		 
		inline static void showMouse(bool show) { instance->showMouseImpl(show); };
		inline static CVec2 mousePosition() { return instance->mousePositionImpl(); };
		inline static float mouseY() { return instance->mouseYImpl(); };
		inline static float mouseX() { return instance->mouseXImpl(); };

		
		// if not using Singleton
		static Scope<Input> Create();

	protected:


		virtual CVec2 mousePositionImpl() = 0;
		virtual float mouseYImpl() = 0;
		virtual float mouseXImpl() = 0;
		virtual void showMouseImpl(bool show) = 0;
		virtual bool isKeyPressedImpl(KeyCode key) = 0;
		virtual bool isKeyReleasedImpl(KeyCode key) = 0;
		virtual bool isKeyRepeatedImpl(KeyCode key) = 0;

		virtual bool isMouseButtonPressedImpl(KeyCode key) = 0;
		virtual bool isMouseButtonReleasedImpl(KeyCode key) = 0;

		static Input* instance;

	};

	class WindowProps
	{
	public:	

		WindowProps(const std::string& title = "Clover Engine", u32 width = 1280, u32 height = 720)
			: title_(title), width_(width), height_(height)
		{
		};

		~WindowProps() {};

		CString title_;
		u32 width_;
		u32 height_;
		bool close = false;
	};


	class Window
	{
	public:

		static Window* GetCurrentWindow();

		virtual void shutdown() = 0;
		virtual bool isOpen() = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;
		virtual void OnUpdate() = 0;

		virtual inline u32 width() const = 0;
		virtual inline u32 height() const = 0;

		virtual inline void* GetNativeWindow() = 0;

		CVec2 MapToViewport(const CVec2& Position)
		{
			return { 2.f * ((Position.x / data_.width_)) - 1.f,
				(2.f * (Position.y / data_.height_)) - 1.f };
		}

		Window() {};
		~Window()
		{};

	public:
		// Delegates
		OnKeyTypedEvent OnKeyTyped;
		OnKeyPressedEvent OnKeyPressed;
		OnKeyReleasedEvent OnKeyReleased;

		OnMouseMovedEvent OnMouseMoved;
		OnMouseScrolledEvent OnMouseScrolled;
		OnMouseButtonPressedEvent OnMouseButtonPressed;
		OnMouseButtonReleasedEvent OnMouseButtonReleased;

		OnWindowCloseEvent OnWindowClose;
		OnWindowResizeEvent OnWindowResize;
		OnWindowFocusEvent OnWindowFocus;
		OnWindowLostFocusEvent OnWindowLostFocus;
		OnWindowMovedEvent OnWindowMoved;

	protected:
		struct WindowData
		{
			u32 width_, height_;
			CString title;
			bool open;
			bool vSync;
		} data_;


		Ref<GraphicContext> context_;

	};

#endif
