#include "UI.h"
#include "Graphics/renderer2D.h"
#include "Graphics/camera.h"
#include "Graphics/window.h"

static Ref<Window> s_Window;
static SceneCamera* s_Camera;
static std::vector<UIObject*> s_UIObjects;

void UI::Init(Ref<Window> window, SceneCamera* camera)
{
	s_Window = window;
	s_Camera = camera;
	TextRenderer::SetFont(new Font("assets/fonts/charmap-oldschool_white.png", { 18, 6 }, { 7, 9 }));
}

void UI::RegisterUIObject(UIButton& button)
{
	s_Window->OnMouseMoved.AddFunction(&button, &UIButton::CheckMouse);
	s_Window->OnMouseButtonPressed.AddFunction(&button, &UIButton::CheckMousePressed);
	s_Window->OnMouseButtonReleased.AddFunction(&button, &UIButton::CheckMouseReleased);

	s_UIObjects.push_back(&button);
}

void UI::Text(const std::string& text, const glm::vec2& position, const glm::vec2& spacing, const glm::vec2& scale /*= glm::vec2(1.0f)*/, const glm::vec4& color /*= glm::vec4(1.0f)*/)
{
	TextRenderer::RenderText(text, position - s_Camera->GetOrthographicSize(), spacing, scale, color);
}

void UI::Render(UIObject& object)
{
	TexRenderable& renderable = object.GetCurrentRenderable();
	Renderer2D::DrawRotatedQuad(renderable.Position - s_Camera->GetOrthographicSize(), renderable.Size, 
		renderable.Rotation, renderable.Texture, renderable.TilingFactor, renderable.Color);
}

UIButton::UIButton()
{
	Setup({ 0, 0 }, { 30, 30 }, 0.f, { 1, 1, 1, 1 }, Renderer2D::GetWhiteTexture());
}

void UIButton::Setup(const glm::vec2 position, const glm::vec2 size, const float rotation, const glm::vec4 tint, Ref<Texture2D>& texture)
{
	m_Position = position;
	m_Bounds = size;

	m_States[(u8)UIButton::State::Default].Position = position;
	m_States[(u8)UIButton::State::Default].Size = size;
	m_States[(u8)UIButton::State::Default].Rotation = rotation;
	m_States[(u8)UIButton::State::Default].Color = tint;
	m_States[(u8)UIButton::State::Default].Texture = texture;

	m_States[(u8)UIButton::State::Hovered].Position = position;
	m_States[(u8)UIButton::State::Hovered].Size = size;
	m_States[(u8)UIButton::State::Hovered].Rotation = rotation;
	m_States[(u8)UIButton::State::Hovered].Color = tint;
	m_States[(u8)UIButton::State::Hovered].Texture = texture;

	m_States[(u8)UIButton::State::Pressed].Position = position;
	m_States[(u8)UIButton::State::Pressed].Size = size;
	m_States[(u8)UIButton::State::Pressed].Rotation = rotation;
	m_States[(u8)UIButton::State::Pressed].Color = tint;
	m_States[(u8)UIButton::State::Pressed].Texture = texture;
}

void UIButton::CheckMouse(float mouseX, float mouseY)
{
	glm::vec2 MousePos = s_Camera->ScreenToWorld(glm::vec2{ 1.f, -1.f } *s_Window->MapToViewport({ mouseX, mouseY }));

	if (Contains(MousePos))
	{
		if (CurrentState == State::Default)
		{
			m_OnHover.Broadcast();
			CurrentState = State::Hovered;
		}
	}
	else if(CurrentState == State::Hovered)
	{
		CurrentState = State::Default;
		m_OnHoverOut.Broadcast();
	}

	LastMousePos = MousePos;
}

void UIButton::CheckMousePressed(int button)
{
	if (Contains(LastMousePos))
	{
		CurrentState = State::Pressed;
		m_OnPressed.Broadcast();
	}
}

void UIButton::CheckMouseReleased(int button)
{
	if (CurrentState == State::Pressed)
	{
		m_OnReleased.Broadcast();
	}

	if (Contains(LastMousePos))
	{
		CurrentState = State::Hovered;
	}
	else
	{
		CurrentState = State::Default;
	}
}

bool UIButton::Contains(const glm::vec2& Point) const
{
	return (Point.x >= (m_Position.x - s_Camera->GetOrthographicSize()) - (m_Bounds.x * 0.5f) &&
			Point.x <= (m_Position.x - s_Camera->GetOrthographicSize()) + (m_Bounds.x * 0.5f) &&
			Point.y >= (m_Position.y - s_Camera->GetOrthographicSize()) - (m_Bounds.y * 0.5f) &&
			Point.y <= (m_Position.y - s_Camera->GetOrthographicSize()) + (m_Bounds.y * 0.5f));
}