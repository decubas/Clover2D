#include "Card.h"
#include "../PrototypeMain.h"

CardBase::CardBase(const CardDescription& description)
	: Description(description)
{
	TitlePos = { 20, 20 };
	
	m_Bounds = { 1.f, 1.f };
}

void CardBase::Update(const float deltaTime, const glm::vec2& MousePos)
{

}

void CardBase::Draw(const glm::vec2& MousePos)
{
	RenderInfo.Position = m_Position + m_Bounds * 0.5f;
	RenderInfo.Size = m_Bounds;
	Renderer2D::DrawQuad(RenderInfo);
	TextRenderer::RenderText(Description.Name, Center(), { 0, 0 }, {5, 5});

	glm::vec4 BoxColor = { 0, 1, 0, 1 };

	if (Contains(MousePos))
	{
		BoxColor = { 1, 0, 0, 1 };
	}

	Renderer2D::DrawLine(TopLeft(), TopRight(), 5.f, BoxColor);
	Renderer2D::DrawLine(TopRight(), BottomRight(), 5.f, BoxColor);
	Renderer2D::DrawLine(BottomRight(), BottomLeft(), 5.f, BoxColor);
	Renderer2D::DrawLine(BottomLeft(), TopLeft(), 5.f, BoxColor);
}

void CardBase::OnMouseDown()
{
}

void CardBase::OnMouseReleased()
{
}

Draggable::Draggable()
{
	Window* window = Window::GetCurrentWindow();
	window->OnMouseButtonPressed.AddFunction(this, &Draggable::OnMouseDown);
	m_Offset = { 0.0f, 0.0f };
}

void Draggable::CheckMouse(float mouseX, float mouseY)
{
	//Window* window = Window::GetCurrentWindow();
	//PrototypeCamera& MainCamera = PrototypeEngine::GetEngine()->MainCamera;
	//glm::vec2 MousePos = MainCamera.ScreenToWorld({ Input::mouseX(), Input::mouseY() }, MainCamera.GetPosition());
	//OnMouseDragged(MousePos);
}

void Draggable::OnMouseDown(int button)
{
	/*if (button == Mouse::Button0)
	{
		Window* window = Window::GetCurrentWindow();
		PrototypeCamera& MainCamera = PrototypeEngine::GetEngine()->MainCamera;
		glm::vec2 MousePos = MainCamera.ScreenToWorld({ Input::mouseX(), Input::mouseY() }, MainCamera.GetPosition());

		if (Contains(MousePos))
		{
			m_OnMouseMovedHandle = window->OnMouseMoved.AddFunction(this, &Draggable::CheckMouse);
			m_OnMouseReleasedHandle = window->OnMouseButtonReleased.AddFunction(this, &Draggable::OnMouseReleased);
			OnMouseDown();
		}

		m_Offset = m_Position - MousePos;
	}*/
}

void Draggable::OnMouseReleased(int button)
{
	Window* window = Window::GetCurrentWindow();
	window->OnMouseMoved.RemoveFunction(m_OnMouseMovedHandle);
	m_Offset = { 0.0f, 0.0f };
	OnMouseReleased();
	window->OnMouseButtonReleased.RemoveFunction(m_OnMouseReleasedHandle);
}

void Draggable::OnMouseDragged(const glm::vec2& MousePos)
{
	m_Position = MousePos + m_Offset;
}


Slot::Slot()
{
	Window* window = Window::GetCurrentWindow();
	window->OnMouseButtonReleased.AddFunction(this, &Slot::OnMouseReleased);
	m_Card = nullptr;
	m_TimerHandle.bLoop = true;
	m_TimerHandle.Tick.AddFunction(this, &Slot::OnUpdate);
}

void Slot::OnUpdate()
{
	m_Card->m_Position = Center() - m_Card->m_Bounds * 0.5f;

}

void Slot::SetCard(CardBase* card)
{
	if (card == m_Card)
		return;

	//PrototypeEngine* engine = PrototypeEngine::GetEngine();
	//engine->m_TimerManager.AddTimer(m_TimerHandle);
	m_Card = card;
	m_Card->m_Position = Center() - m_Card->m_Bounds * 0.5f;
}

void Slot::Draw(const glm::vec2& MousePos)
{
	glm::vec4 BoxColor = { 0, 0, 1, 1 };

	if (Contains(MousePos))
	{
		BoxColor = { 1, 0, 0, 1 };
	}

	Renderer2D::DrawLine(TopLeft(), TopRight(), 2.f, BoxColor);
	Renderer2D::DrawLine(TopRight(), BottomRight(), 2.f, BoxColor);
	Renderer2D::DrawLine(BottomRight(), BottomLeft(), 2.f, BoxColor);
	Renderer2D::DrawLine(BottomLeft(), TopLeft(), 2.f, BoxColor);
}

void Slot::OnMouseReleased(int button)
{
	
}

Button::Button()
{
	Window* window = Window::GetCurrentWindow();
	window->OnMouseButtonPressed.AddFunction(this, &Button::OnMouseDown);
	window->OnMouseButtonReleased.AddFunction(this, &Button::OnMouseReleased);

}

void Button::OnMouseDown(int button)
{
	/*if (button == Mouse::Button0)
	{
		Window* window = Window::GetCurrentWindow();
		PrototypeCamera& MainCamera = PrototypeEngine::GetEngine()->MainCamera;
		glm::vec2 MousePos = MainCamera.ScreenToWorld({ Input::mouseX(), Input::mouseY() }, MainCamera.GetPosition());

		if (Contains(MousePos))
		{
			m_OnPressed.Broadcast(button);
		}
	}*/
}
 
void Button::OnMouseReleased(int button)
{
	/*if (button == Mouse::Button0)
	{
		Window* window = Window::GetCurrentWindow();
		PrototypeCamera& MainCamera = PrototypeEngine::GetEngine()->MainCamera;
		glm::vec2 MousePos = MainCamera.ScreenToWorld({ Input::mouseX(), Input::mouseY() }, MainCamera.GetPosition());
		m_OnReleased.Broadcast(button);
	}*/
}

void ProgressBar::OnDraw()
{

	Renderer2D::DrawQuad(m_Position + glm::vec2(m_Bounds.x * 0.5f, m_Bounds.y * 0.5f), m_Bounds, {1, 1, 1, 1});

	const float Width = (Value / Max) * m_Bounds.x;
	const bool bFilled = Width > m_Bounds.x;
	Renderer2D::DrawQuad(m_Position + glm::vec2(bFilled ? m_Bounds.x * 0.5f : Width * 0.5f , m_Bounds.y * 0.5f), { bFilled ? m_Bounds.x : Width, m_Bounds.y }, {0.2f, 0.3f, 0.9f, 1.f});
}
