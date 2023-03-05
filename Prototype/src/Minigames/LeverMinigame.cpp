#include "LeverMinigame.h"

LeverMinigame::LeverMinigame()
{

	LeverOffset = { 0, 0 };
	m_Bounds = { 100, 200 };

	Lever.Min = 25.f;
	Lever.Max = 187.5f;
	Lever.m_Position = { 0, 0 };
	Lever.m_Bounds = { 100, 200 };
	Lever.gfxPosition = Lever.Center();
	Lever.gfxPosition.y = Lever.m_Position.y;
}

void LeverMinigame::OnUpdate(float fDeltaTime)
{
	Lever.Offset = Lever.Offset <= 0.f ? 0 : Lever.Offset - Lever.RecoverySpeed * fDeltaTime;
}

void LeverMinigame::OnDraw()
{
	Renderer2D::DrawQuad(Center(), m_Bounds, { 0.6, 0.5, 0.8, 1 });
	Lever.m_Position = m_Position + LeverOffset;
	Lever.OnDraw();
}

void LeverDraggable::OnMouseDragged(const glm::vec2& MousePos)
{
	const float yOffset = (MousePos.y - (m_Position.y + m_Offset.y)) / Max;

	Offset += yOffset > 0 ? Speed : 0.f;

	if (Offset >= (Max / m_Bounds.y))
		Offset = (Max / m_Bounds.y);
}

void LeverDraggable::OnDraw()
{
	gfxPosition.x = Center().x;
	gfxPosition.y = m_Position.y + (Offset * Max) + 12.5f;
	Renderer2D::DrawQuad(gfxPosition, { m_Bounds.x, 25.f }, { 0, 1, 0, 1 });
}
