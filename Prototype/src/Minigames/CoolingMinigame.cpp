#include "CoolingMinigame.h"
#include "Graphics/UI.h"

CoolingMinigame::CoolingMinigame()
{

	const float width = 48 * 2;
	const float height = 64 * 2;
	m_Button.m_OnPressed.AddFunction(this, &CoolingMinigame::OnMouseDown);
	m_Button.m_OnReleased.AddFunction(this, &CoolingMinigame::OnMouseReleased);

	ButtonOffset = { 0, 25 };
	BarOffset = { 0, 0 };

	m_Button.m_Bounds = { width, height };
	m_Bounds = { width, height + ButtonOffset.y };
	Bar.m_Bounds = { width, ButtonOffset.y };

	Bar.Max = 100;
}

void CoolingMinigame::OnUpdate(float fDeltaTime)
{
	if (m_bIsActive)
	{
		m_fTemperature -= 5.f * fDeltaTime;
		m_fTemperature = m_fTemperature <= 0.f ? 0 : m_fTemperature;
	}
	else
	{
		m_fTemperature += 2.5f * fDeltaTime;
	}

	Bar.Value = m_fTemperature;
}

void CoolingMinigame::OnDraw()
{
	Renderer2D::DrawQuad(Center(), m_Bounds, {0.6, 0.5, 0.8, 1});

	m_Button.m_Position = m_Position + ButtonOffset;
	Renderer2D::DrawQuad(m_Button.Center(), m_Button.m_Bounds, m_bIsActive ? m_ButtonPressedTexture : m_ButtonTexture);

	Bar.m_Position = m_Position + BarOffset;
	Bar.OnDraw();


	/*m_Button.DrawDebug();
	Bar.DrawDebug();
	DrawDebug();*/
}

void CoolingMinigame::OnMouseDown(int button)
{
	OnActivate();
}

void CoolingMinigame::OnMouseReleased(int button)
{
	m_bIsActive = false;
}
