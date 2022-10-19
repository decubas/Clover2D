#ifndef _Delegate_H_
#define _Delegate_H_ 1

#include <Clover.h>
#include "Tools/Log.h"
#include "DelegateBase.h"

/*
	EXAMPLES:

	Delegate_OneParam(OnKeyPressedEvent, int, Key)
	Delegate_TwoParams(OnKeyPressedEventTwo, int, Key, float, Mat)
	Delegate_ReturnTwoParams(OnKeyPressedEventTwo_Return, bool, int, Key, float, Mat)

	OnKeyPressedEvent m_OnKeyPressed;
	OnKeyPressedEventTwo m_OnKeyPressedTwo;
	OnKeyPressedEventTwo_Return m_OnKeyPressedTwo_Return;

	{
		m_OnKeyPressed.AddFunction(this, &MyModule::OnKeyPressed);
		m_OnKeyPressedTwo.AddFunction(this, &MyModule::OnKeyPressedTwo);
		m_OnKeyPressedTwo_Return.AddFunction(this, &MyModule::OnKeyPressedTwo_Return);

		m_OnKeyPressed.Broadcast(4);
		m_OnKeyPressedTwo.Broadcast(1, 10.5f);
		if (false == m_OnKeyPressedTwo_Return.Broadcast(5,2))
		{
			Cl_DebugLog("On key pressed TWO return FALSE");
		}
	}
*/

Delegate_OneParam(OnKeyTypedEvent, s32, Key)
Delegate_TwoParams(OnKeyPressedEvent, s32, Key, s32, RepeatCount)
Delegate_OneParam(OnKeyReleasedEvent, s32, Key)
	
Delegate_TwoParams(OnMouseMovedEvent, float, x, float, y)
Delegate_TwoParams(OnMouseScrolledEvent, float, x, float, y)
Delegate_OneParam(OnMouseButtonPressedEvent, int, Button)
Delegate_OneParam(OnMouseButtonReleasedEvent, int, Button)

Delegate_TwoParams(OnWindowResizeEvent, float, Width, float, Height)

Delegate(OnWindowCloseEvent)
Delegate(OnWindowFocusEvent)
Delegate(OnWindowLostFocusEvent)
Delegate(OnWindowMovedEvent)

#endif
