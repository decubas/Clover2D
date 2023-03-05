#pragma once

#include <Clover.h>
#include "Graphics/camera.h"
#include "Graphics/renderer2D.h"
#include "Sandbox/rectangle.h"
#include "Tools/Timer.h"

struct CardDescription 
{
	std::string Name;
	float Value;
};

Delegate(OnDraggablePressedEvent)
Delegate_OneParam(OnDraggableDragEvent, const glm::vec2&, MousePos)

class Draggable : public Box2D
{
public:
	Draggable();

	virtual void CheckMouse(float mouseX, float mouseY);
	virtual void OnMouseDragged(const glm::vec2& MousePos);

	virtual void OnMouseDown() {};
	virtual void OnMouseReleased() {};

private:
	void OnMouseDown(int button);
	void OnMouseReleased(int button);
protected:
	OnDraggableDragEvent m_OnDrag;
	OnDraggablePressedEvent m_OnPressed;
	DelegateHandle m_OnMouseReleasedHandle, m_OnMouseMovedHandle;
	glm::vec2 m_Offset;

};

class ProgressBar : public Box2D
{
public:

	void OnDraw();

	Ref<SubTexture2D> Background;
	Ref<SubTexture2D> FillTexture;
	float Value = 0.f;
	float Max = 1.f;

};


class Button : public Box2D
{
public:
	Button();

	OnMouseButtonPressedEvent m_OnPressed;
	OnMouseButtonReleasedEvent m_OnReleased;
private:
	void OnMouseDown(int button);
	void OnMouseReleased(int button);
};

class CardBase : public Draggable
{
public:
	CardBase() = default;
	CardBase(const CardDescription& description);

	void Update(const float deltaTime, const glm::vec2& MousePos);
	void Draw(const glm::vec2& MousePos);

	virtual void OnMouseDown() override;
	virtual void OnMouseReleased() override;

	CardDescription& GetDescription() { return Description; }

	SubTexRenderable RenderInfo;
	glm::vec2 TitlePos;
protected:
	CardDescription Description;
};


class Slot : public Box2D
{
public:
	Slot();
	void OnUpdate();
	void Draw(const glm::vec2& MousePos);

	void OnMouseReleased(int button);

	void SetCard(CardBase* card);
protected:
	OnMouseButtonReleasedEvent m_OnMouseReleased;
	TimerHandler m_TimerHandle;
	CardBase* m_Card;
};
