#pragma once

#include "PrototypeMain.h"
#include "Entities/Card.h"
#include "Entities/Minigame.h"
#include "Player/Player.h"

#include "Minigames/CoolingMinigame.h"
#include "Minigames/LeverMinigame.h"
#include "Editor/EditorLayer.h"

static bool s_bImGuiEditorOpen = false;
static glm::vec2 MousePos;

void PrototypeEngine::EnableEditor(s32 key)
{
	if (key == Key::F1)
	{
		s_bImGuiEditorOpen = !s_bImGuiEditorOpen;
	}
}

void PrototypeEngine::OnUserConstruct()
{
	m_Window->OnKeyReleased.AddFunction(this, &PrototypeEngine::EnableEditor);
	m_Window->OnWindowResize.AddFunction(&MainCamera, &PrototypeCamera::OnResize);
	MainCamera.m_Transform.m_Position.z = 1;

	ButtonPressedAtlas = GraphicContext::CreateTexture2D(ASSETS_PATH("textures/Buttons2.png"));
	ButtonPressedAtlas->setMagFilter(Texture::Filter::NEAREST);
	ButtonAtlas = GraphicContext::CreateTexture2D(ASSETS_PATH("textures/Buttons1.png"));
	ButtonAtlas->setMagFilter(Texture::Filter::NEAREST);

	Floor.Texture = GraphicContext::CreateTexture2D(ASSETS_PATH("textures/Checkerboard.png"));
	Floor.Texture->setMagFilter(Texture::Filter::NEAREST);
	Floor.Size = { 5000.f, 5000.f };
	Floor.TilingFactor = 10.f;
	Floor.Color = { 0.2f, 0.2f, 0.2f, 1.f };



	CoolingMinigame* coolingMg = new CoolingMinigame();
	coolingMg->m_Position = { -100, 100 };
	coolingMg->m_ButtonTexture = GraphicContext::CreateSubTexture2D(ButtonAtlas, {2, 0}, { 47.25f, 64 });
	coolingMg->m_ButtonPressedTexture = GraphicContext::CreateSubTexture2D(ButtonPressedAtlas, {2, 0}, { 47.25f, 64 });

	m_ActiveMinigames.emplace_back(coolingMg);
		

	LeverMinigame* leverMg = new LeverMinigame();
	leverMg->m_Position = { 100, 100 };
	m_ActiveMinigames.emplace_back(leverMg);


}

void PrototypeEngine::OnUserUpdate(float fDeltaTime)
{
	MainCamera.OnUpdate(fDeltaTime, 500.f);

	glm::vec2 MousePosition = MainCamera.ScreenToWorld({ Input::mouseX(), Input::mouseY() }, MainCamera.GetPosition());
	MousePos = { Input::mouseX(), Input::mouseY() };

	Renderer2D::BeginScene(MainCamera, MainCamera.m_Transform.RecalculateModel());
	Renderer2D::Clear({ 0, 0, 0, 1 });

	Renderer2D::DrawQuad(Floor);


	m_ActiveMinigames[0]->OnDraw();
	m_ActiveMinigames[1]->OnDraw();

	Renderer2D::DrawQuad({ 0, 0 }, { 10, 10 }, { 1, 0, 0, 1 });
	Renderer2D::EndScene();
}

void PrototypeEngine::OnUserUI(float fDeltaTime)
{
	char str[128] = {0};
	snprintf(str, 128, "%.1f, %.1f", MousePos.x, MousePos.y);
	UI::Text(str, {1.5f, 5.f}, {1, 1}, {0.5, 0.5});
}

void PrototypeEngine::OnUserImGuiUpdate(float fDeltaTime)
{
	if (!s_bImGuiEditorOpen)
	{
		return;
	}

	ImGuiShowDrawStats();

	ImGui::Begin("Viewport");
	ImGui::Text("Delta %f", fDeltaTime);
	ImGui::Image((void*)Renderer2D::GetLastTexture()->id(), ImVec2{ (float)m_Window->width(), (float)m_Window->height() }, { 0, 1 }, { 1, 0 });
	ImGui::End();
}


void PrototypeEngine::OnUserDestroy() 
{
	for (Minigame* mg : m_ActiveMinigames)
		delete mg;
}


static CloverEngine* Sandbox;

void main()
{
	Sandbox = new CloverEngine();
	
	if (!Sandbox->Construct(WindowProps("Sandbox Window", 1280, 720), true))
		return;

	Sandbox->m_LayerStack.pushLayer(CreateRef<EditorLayer>());
	Sandbox->Run();

	delete Sandbox;
}

CloverEngine* CloverEngine::GetEngine()
{
	return Sandbox;
}
