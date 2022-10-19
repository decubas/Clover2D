#ifndef _WINDOWS_WINDOW_H__
#define _WINDOWS_WINDOW_H__ 1


#include "Graphics/window.h"


	class WindowsInput : public Input
	{
	public :
		WindowsInput(){}
		~WindowsInput(){}

	protected:
		bool isKeyPressedImpl(KeyCode key) override;
		bool isKeyReleasedImpl(KeyCode key) override;
		bool isKeyRepeatedImpl(KeyCode key) override;

		bool isMouseButtonPressedImpl(KeyCode key) override;
		bool isMouseButtonReleasedImpl(KeyCode key) override;

		void showMouseImpl(bool show) override;
		CVec2 mousePositionImpl() override;
		float mouseYImpl() override;
		float mouseXImpl() override;
	};

	class WindowsWindow : public Window
	{
	public:

		virtual inline void* GetNativeWindow() override { return window_;};

		virtual bool isOpen() override { return window_ != nullptr; };
		virtual void OnUpdate() override;
		virtual void setVSync(bool enabled) override;
		virtual bool isVSync() const override;

		inline u32 width() const override { return data_.width_; };
		inline u32 height() const override { return data_.height_; };

		WindowsWindow(const WindowProps& windowProps);
		virtual ~WindowsWindow();

		virtual void shutdown() override;

	private:
		virtual void Init(const WindowProps& windowProps);
		
		GLFWwindow* window_;

		
		
	};
#endif
