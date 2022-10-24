#pragma once

#define CL_USE_IMGUI
#define SANDBOX 1

#include <CloverEngine.h>
#include "imgui.h"
#include "Graphics/UI.h"
#include "Tools/Serializer.h"

struct Foo
{
	int pepe = 0;
	int pepe1 = 0;
	float pepe2 = 0;
	float pepe3 = 0;

	SERIALIZED_CLASS(
		Foo,
		SERIALIZE_VAR(pepe)
		SERIALIZE_VAR(pepe1)
		SERIALIZE_VAR(pepe2)
		SERIALIZE_VAR(pepe3),

		DESERIALIZE_VAR(pepe)
		DESERIALIZE_VAR(pepe1)
		DESERIALIZE_VAR(pepe2)
		DESERIALIZE_VAR(pepe3),

		SERIALIZED_VAR(pepe),
		SERIALIZED_VAR(pepe1),
		SERIALIZED_VAR(pepe2),
		SERIALIZED_VAR(pepe3),
		);

};

struct Foo2
{
	int manolo = 0;
	int manolo1 = 0;
	float manolo2 = 0;
	float manolo3 = 0;
	Foo foo;

	SERIALIZED_CLASS(
		Foo2,

		SERIALIZE_CHILD(foo)
		SERIALIZE_VAR(manolo)
		SERIALIZE_VAR(manolo1)
		SERIALIZE_VAR(manolo2)
		SERIALIZE_VAR(manolo3),

		DESERIALIZE_CHILD(foo)
		DESERIALIZE_VAR(manolo)
		DESERIALIZE_VAR(manolo1)
		DESERIALIZE_VAR(manolo2)
		DESERIALIZE_VAR(manolo3),

		SERIALIZED_VAR(manolo),
		SERIALIZED_VAR(manolo1),
		SERIALIZED_VAR(manolo2),
		SERIALIZED_VAR(manolo3),
		);
};

class SandboxEngine : public CloverEngine
{

protected:
	Ref<Texture2D> Tiles;
	Ref<Texture2D> Planes;
	Ref<SubTexture2D> Plane1;
	Ref<SubTexture2D> Plane2;
	Ref<SubTexture2D> Plane3;
	Animation2D<SubTexture2D> Anim;
	TexRenderable Checkerboard;

	UIButton TestButton;

	glm::vec4 TextColor = {1, 1, 1, 1};
	char* TextToRender;
	
	CVec2 Position;
	char CharBuffer[1024] = {0};

	void OnButtonPressed()
	{
		Cl_Warning("Button Pressed");
	}

	void OnButtonHoverOut()
	{
		Cl_Info("Button Hover OUT");
	}

	void OnButtonHover()
	{
		Cl_Info("Button Hover");
	}

	void OnUserConstruct() override
	{
		Tiles = GraphicContext::CreateTexture2D("assets/textures/tiles_packed.png");
		Planes = GraphicContext::CreateTexture2D("assets/textures/ships_packed.png");
		Planes->setMagFilter(Texture::Filter::NEAREST);
		Plane1 = GraphicContext::CreateSubTexture2D(Planes, { 0, 2 }, { 32, 32 });
		Plane2 = GraphicContext::CreateSubTexture2D(Planes, { 0, 1 }, {32, 32});
		Plane3 = GraphicContext::CreateSubTexture2D(Planes, { 0, 0 }, {32, 32});

		Anim = Animation2D<SubTexture2D>({ Plane1, Plane2, Plane3 }, 0.05f);

		Checkerboard.Texture = GraphicContext::CreateTexture2D("assets/textures/Checkerboard.png");
		Checkerboard.Texture->setMagFilter(Texture::Filter::NEAREST);
		Checkerboard.Size = { 100.f, 100.f };
		Checkerboard.TilingFactor = 10.f;

		TextToRender = new char[128];
		strcpy_s(TextToRender, sizeof(char) * 128, "Hello \nthere\0");

		float x = 20;
		float y = -20;
		TestButton.Setup({ 20, 20 }, { 30, 30 }, 0.f, { 1, 0, 1, 1 }, Renderer2D::GetWhiteTexture());

		TestButton.m_States[(u8)UIButton::State::Hovered].Size = { 20, 20 };
		TestButton.m_States[(u8)UIButton::State::Pressed].Size = { 15, 15 };

		UI::RegisterUIObject(TestButton);

		TestButton.m_OnHover.AddFunction(this, &SandboxEngine::OnButtonHover);
		TestButton.m_OnHoverOut.AddFunction(this, &SandboxEngine::OnButtonHoverOut);
		TestButton.m_OnPressed.AddFunction(this, &SandboxEngine::OnButtonPressed);

		
		Foo2 foo;
		foo.m_Serialized = JsonSerializer::Load("assets/foo.json");
		foo.Deserialize();

		foo.manolo += 1;
		foo.manolo1 -= 1;
		foo.manolo2 *= 1.25f;
		foo.manolo3 *= 0.75f;

		foo.foo.pepe += 1;
		foo.foo.pepe1 -= 1;
		foo.foo.pepe2 *= 1.25f;
		foo.foo.pepe3 *= 0.75f;

		foo.Serialize();
		JsonSerializer::Write("assets/foo.json", foo.m_Serialized);

	}


	bool bPressed = false;
	glm::vec2 FirstLocation;
	void OnUserUpdate(float fDeltaTime) override
	{
		m_EditorCamera.OnUpdate(fDeltaTime, 3.f);

		Renderer2D::BeginScene(m_EditorCamera);
		Renderer2D::Clear({0, 0, 0, 1});

		Renderer2D::DrawQuad(Checkerboard);

		glm::vec2 MousePos = GetMouseScreenPosition();
		Renderer2D::DrawQuad(MousePos, { 10, 10 }, Anim.GetCurrentTexture());

		glm::vec2 target = { MousePos.x, MousePos.y };


		Renderer2D::EndScene();
	}
	void OnUserUI(float fDeltaTime) override
	{
		UI::Render(TestButton);
		UI::Text(TextToRender, { 50, 25 }, { 0, 0 }, { 1, 1 }, TextColor);
	}

	void OnUserImGuiUpdate(float fDeltaTime) override
	{
		ImGuiShowDrawStats();

		ImGui::Begin("Editor");
		ImGui::Text("Delta %f", fDeltaTime);
		ImGui::DragFloat3("Drag Camera Scale", glm::value_ptr(m_EditorCamera.GetTransform().m_Scale), 0.025f, 0, 1);
		ImGui::ColorEdit4("Text Color", glm::value_ptr(TextColor));
		ImGui::InputTextMultiline("Text To Render", TextToRender, 128);
		ImGui::End();
		
		ImGui::Begin("Viewport");
		ImGui::Image((void*)Renderer2D::GetLastTexture()->id(), ImVec2{ (float)m_Window->width(), (float)m_Window->height() }, { 0, 1 }, { 1, 0 });
		ImGui::End();
	}


	void OnUserDestroy() override
	{
	}

};

#if SANDBOX
void main()
{
	SandboxEngine Sandbox;

	if (Sandbox.Construct(WindowProps("Sandbox Window", 700, 700), true))
		Sandbox.Run();


}
#endif