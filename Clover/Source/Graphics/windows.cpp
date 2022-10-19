#include "Clover.h"


#include <Graphics/window.h>
#include <Window.h>

 
	Input* Input::instance = new WindowsInput();

	Scope<Input> Input::Create()
	{
		return CreateScope<WindowsInput>();
	}
