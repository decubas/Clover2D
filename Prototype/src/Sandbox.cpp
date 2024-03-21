#pragma once

#define CL_USE_IMGUI
#define SANDBOX 0

#include <CloverEngine.h>
#include <stddef.h>
#include "imgui.h"
#include "Grid/Grid.h"
#include "GameplayCamera/GameplayCamera.h"
#include "Graphics/window.h"
#include "Graphics/ImUI.h"
#include "Tools/Tilemap.h"
#include "Tools/Serializer.h"


#if SANDBOX
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
 		Tiles = GraphicContext::CreateTexture2D(ASSETS_PATH("textures/tiles_packed.png"));
 		Planes = GraphicContext::CreateTexture2D(ASSETS_PATH("textures/ships_packed.png"));
 		Planes->setMagFilter(Texture::Filter::NEAREST);
 		Plane1 = GraphicContext::CreateSubTexture2D(Planes, { 0, 2 }, { 32, 32 });
 		Plane2 = GraphicContext::CreateSubTexture2D(Planes, { 0, 1 }, {32, 32});
 		Plane3 = GraphicContext::CreateSubTexture2D(Planes, { 0, 0 }, {32, 32});

		Anim = Animation2D<SubTexture2D>({ Plane1, Plane2, Plane3 }, 0.05f);

		Checkerboard.Texture = GraphicContext::CreateTexture2D(ASSETS_PATH("textures/trex.png"));
		Checkerboard.Texture->setMagFilter(Texture::Filter::NEAREST);
		Checkerboard.Size = { 100.f, 100.f };
		Checkerboard.TilingFactor = 1.f;

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
		foo.m_Serialized = JsonSerializer::Load("../assets/foo.json");
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

		//ImGui::ShowDemoWindow();

		static char buffer[256] = { 0 };
		static int current = 0;
		const char* items[3] = { "item1", "item2", "item3" };
		static float prop_float = 0.f;
		static float prop_float2[2] = {0.f};
		static float prop_float3[3] = {0.f};
		static float prop_colour3[3] = {0.f};
		static float prop_colour4[4] = {0.f};
		static int prop_int = 0;
		static bool prop_bool = false;
		static std::string prop_string = "false";

		ImUI::BeginWindow("Test");
		ImUI::BeginPropertyGrid();
		
		ImUI::Property("float prop", prop_float);
		ImUI::Property("Float2", prop_float2);
		ImUI::Property("Float3", prop_float3);
		ImUI::Property("int prop", prop_int);
		ImUI::Property("bool prop", prop_bool);
		ImUI::Property("string prop", prop_string);
		ImUI::Property("text prop", "test text");

		ImUI::PropertyVec2("Vec2", prop_float2, 0);
		ImUI::PropertyVec3("Vec3", prop_float3, 0);
		ImUI::PropertyColor("Color3", prop_colour3, false);
		ImUI::PropertyColor("Color4", prop_colour4);
		ImUI::PropertySlider("prop slider", prop_int, 0, 10);
		ImUI::PropertyText("text prop", "test text");
		ImUI::PropertySelection("selec prop", current, items, 3);
		//ImUI::PropertySelectionSearch("Selection Search", current, items, IM_ARRAYSIZE(items));
		//ImUI::SimpleComboSearch("Simple Search", current, items, IM_ARRAYSIZE(items));
		//ImUI::ComboAutoSelect("ComboAutoSelect", buffer, 256, &current, items, IM_ARRAYSIZE(items));
		ImUI::EndPropertyGrid();
		ImUI::EndWindow();
		


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

void main()
{
	SandboxEngine Sandbox;

	if (Sandbox.Construct(WindowProps("Sandbox Window", 700, 700), true))
		Sandbox.Run();


}
#else

struct Foo2
{
	Foo2() = default;
	int a = 10, b = 11, c = 12;
};

class Foo : public Class<Foo>
{
public:
	Foo() = default;
	
	PROPERTY(int, a);
	int a = 1;

	PROPERTY(int, b);
	int b = 2;

	PROPERTY(int, c);
	int c = 3;

};


class WorldEngine : public CloverEngine
{

protected:

	struct Stats
	{
		float DrawTime = 0;
		float CameraUpdateTime = 0;

	};
	Stats m_Stats;
	Clock Timer;


	CGrid Map;
	bool bShowGrid = true;
	GameplayCamera GameCamera;
	const uint32 CellSize = 16;

	glm::vec2 worldPos;

	Ref<SubTexture2D> Tiles[2];
	

	Ref<Texture2D> Tile;
	Tilemap TMap = Tilemap(100, 100);


	

	void OnUserConstruct() override
	{
		Map.Generate(100, 100, CellSize, 0, 0);

		GameCamera.SetOrthographic(100, -1, 1);

		m_Window->OnMouseScrolled.AddFunction(&GameCamera, &GameplayCamera::OnScroll);

		Image image[2] = { Image("../assets/textures/tiles_packed.png"), Image("../assets/textures/Checkerboard.png") };


		for(int i = 0; i < 50; i++)
			TMap.SetTile(i, i, {1, nullptr});

		TMap.Build(16, 16, image, 1, Tile, false);
	}

	void OnUserUpdate(float fDeltaTime) override
	{
		Timer.reset();
		float time = 0;
		GameCamera.Update(fDeltaTime, 200, 20, 0.9f);
		m_Stats.CameraUpdateTime = Timer.get();

		Renderer2D::BeginScene(GameCamera, GameCamera.GetTransform().RecalculateModel());
		Renderer2D::Clear({ 0, 0, 0, 1 });

// 		Renderer2D::DrawLine({ 0, 0 }, { 10, 0}, 1.f, { 1, 0, 0, 1 });
// 		Renderer2D::DrawLine({ 0, 0 }, { 0, 10}, 1.f, { 0, 1, 0, 1 });

		Renderer2D::DrawQuad({ -10, 0 }, { TMap.Width * 64, TMap.Height * 64 }, Tile);

		if(bShowGrid)
			DrawGrid();


		Renderer2D::EndScene();
		m_Stats.DrawTime = Timer.get();
	}

	void OnUserUI(float fDeltaTime) override
	{
	}

	void OnUserImGuiUpdate(float fDeltaTime) override
	{
		ImGuiShowDrawStats();
		ImGui::Begin("Settings");
		ImGui::Text("Delta %f", fDeltaTime);
		ImGui::Text("FPS %f", 1.f/fDeltaTime);
		ImGui::Text("Draw %f", m_Stats.DrawTime);
		ImGui::Text("Camera Update %f", m_Stats.CameraUpdateTime);
		
		CVec2 MousePos = GameCamera.GetWorldMousePos();
		ImGui::Text("Mouse Pos %f %f", MousePos.x, MousePos.y);
		ImGui::Text("Camera Pos %f %f", worldPos.x, worldPos.y);
		ImGui::Text("Zoom %f", GameCamera.GetOrthographicSize());

		ImUI::Property("Show Grid", bShowGrid);

		CGrid::SCell cell;
		if (Map.GetCellFromWorldLocation(MousePos.x, MousePos.y, cell))
		{
			ImGui::Text("Cell %d %d, %d", cell.index%Map.GetWidth(), cell.index/Map.GetWidth(), cell.Size);
		}
		ImGui::End();
	}


	void OnUserDestroy() override
	{
	}

	void DrawGrid()
	{
		const float Size = Map.GetCellSize();
		const float HalfSize = Map.GetCellSize() * 0.5f;
		const float off_x = Map.GetOffsetX();
		const float off_y = Map.GetOffsetY();

//		for (uint32 i = 0; i <= Map.GetWidth(); i++)
// 		{
// 			Renderer2D::DrawLine({ i * Size - HalfSize + off_x, -HalfSize + off_y}, { i * Size - HalfSize + off_x, Map.GetHeight() * Size - HalfSize + off_y}, 1.f, { 1, 1, 1, .5 });
// 			Renderer2D::DrawLine({ -HalfSize + off_x, i * Size - HalfSize + off_y }, { Map.GetWidth() * Size - HalfSize + off_x, i * Size - HalfSize + off_y }, 1.f, { 1, 1, 1, .5 });
// 		}
		
		CVec2 MousePos = GameCamera.GetWorldMousePos();
		float size = 50;
		
		const Box2D AABB = Box2D(MousePos.x - size * 0.5f, MousePos.y - size * 0.5f, size, size);

		glm::vec4 color = { 1, 0, 0 ,1 };
		const float thickness = 1.f;
// 		Renderer2D::DrawLine(AABB.TopLeft(), AABB.TopRight(), thickness, color);
// 		Renderer2D::DrawLine(AABB.TopRight(), AABB.BottomRight(), thickness, color);
// 		Renderer2D::DrawLine(AABB.BottomRight(), AABB.BottomLeft(), thickness, color);
// 		Renderer2D::DrawLine(AABB.BottomLeft(), AABB.TopLeft(), thickness, color);
		



//		for (const CGrid::SCell& Cell : Map.GetCells())
//		{
//			const Box2D Tile_AABB = Box2D(Cell.x - Cell.Size * 0.5f, Cell.y - Cell.Size * 0.5f, Cell.Size, Cell.Size);
//
//			if (GameCamera.IsInFrustum(Tile_AABB))
//			{
//				Renderer2D::DrawLine(Tile_AABB.TopLeft(), Tile_AABB.TopRight(), thickness, color);
//				Renderer2D::DrawLine(Tile_AABB.TopRight(), Tile_AABB.BottomRight(), thickness, color);
//				Renderer2D::DrawLine(Tile_AABB.BottomRight(), Tile_AABB.BottomLeft(), thickness, color);
//				Renderer2D::DrawLine(Tile_AABB.BottomLeft(), Tile_AABB.TopLeft(), thickness, color);
//				Renderer2D::DrawQuad({ Cell.x, Cell.y }, { 1, 1 }, Tiles[1]);
//			}
//		}



		// Camera Frustum
		worldPos = (GameCamera.ScreenToWorld({0, 0}));
		//Renderer2D::DrawQuad(worldPos, { 10, 10 }, {0, 1, 0, 1});
		const Box2D Frustum = Box2D(
			worldPos.x - GameCamera.GetOrthographicSize(), worldPos.y - GameCamera.GetOrthographicSize(),
			GameCamera.GetOrthographicSize() * 2.f, GameCamera.GetOrthographicSize() * 2.f);
// 		Renderer2D::DrawLine(Frustum.TopLeft(), Frustum.TopRight(), thickness, color);
// 		Renderer2D::DrawLine(Frustum.TopRight(), Frustum.BottomRight(), thickness, color);
// 		Renderer2D::DrawLine(Frustum.BottomRight(), Frustum.BottomLeft(), thickness, color);
// 		Renderer2D::DrawLine(Frustum.BottomLeft(), Frustum.TopLeft(), thickness, color);

		// Intersected tile
	/*	CGrid::SCell cell;
		if (Map.GetCellFromWorldLocation(MousePos.x, MousePos.y, cell))
		{
			Renderer2D::DrawQuad({ cell.x, cell.y }, { cell.Size, cell.Size }, { 1, 1, 1, .2 });
		}
*/



	}

};

void main()
{
	/*Foo f;


	printf("Class num prop %d\n", Foo::PropertyMap.size());
	printf("Offset %d %d %d\n", Foo::PropertyMap["a"].GetOffset(), Foo::PropertyMap["b"].GetOffset(), Foo::PropertyMap["c"].GetOffset());
	printf("Data %d %d %d\n", Foo::GetValue<int>(&f, "a"), Foo::GetValue<int>(&f, "b"), Foo::GetValue<int>(&f, "c"));
	printf("Data as string %s \n", f.Property_b.ToString().c_str());
	
	JsonSerializer::JsonObject json = Serializer::Serialize<Foo>(&f, "f");
	JsonSerializer::Print(json);
	return;*/








	WorldEngine World;

	if (World.Construct(WindowProps("World", 700, 700), true))
		World.Run();


}

#endif