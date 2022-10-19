#pragma once

#ifndef _UI_
#define _UI_ 1

#include <Clover.h>
#include "Delegate/Delegate.h"
#include "Graphics/renderer2D.h"
#include "Sandbox/rectangle.h"

class EditorCamera;
class Window;

Delegate(OnUIObjectPressedEvent)
Delegate(OnUIObjectHoverEvent)



class UIObject : public Cl_Object
{
public:
	glm::vec2 m_Position;
	glm::vec2 m_Bounds;

	virtual TexRenderable& GetCurrentRenderable() { return m_Renderable; }

protected:
	TexRenderable m_Renderable;
};

class UIButton : public UIObject
{
public:
	UIButton();

	enum class State : u8
	{
		Default = 0,
		Hovered = 1,
		Pressed = 2,

		MaxState = 3
	};

	void Setup(const glm::vec2 position, const glm::vec2 size, const float rotation, const glm::vec4 tint, Ref<Texture2D>& texture );

	TexRenderable m_States[(u8)State::MaxState];
	State GetCurrentState() { return CurrentState; }
	TexRenderable& GetCurrentRenderable() override { return m_States[(u8)CurrentState]; }

	void CheckMouse(float mouseX, float mouseY);
	void CheckMousePressed(int button);
	void CheckMouseReleased(int button);

	bool Contains(const glm::vec2& Point) const;

	OnUIObjectPressedEvent m_OnPressed;
	OnUIObjectPressedEvent m_OnReleased;
	OnUIObjectHoverEvent m_OnHover;
	OnUIObjectHoverEvent m_OnHoverOut;


private:
	State CurrentState = UIButton::State::Default;
	glm::vec2 LastMousePos;
};

class UI
{
public:
	static void Init(Ref<Window> window, SceneCamera* camera);

	static void RegisterUIObject(UIButton& button);

	static void Text(const std::string& text, const glm::vec2& position, const glm::vec2& spacing = glm::vec2(1.0f),
		const glm::vec2& scale = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f));

	static void Render(UIObject& object);
};


#endif