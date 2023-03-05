#pragma once

#ifndef _RENDERER2D_H
#define _RENDERER2D_H

#include <Clover.h>

#include <Utils/Library.h>
#include <Tools/CommandList.h>

#include "graphiccontext.h"
#include "camera.h"
#include "renderer.h"
#include "font.h"

struct RenderableInfo
{
	CVec2 Position = {0.f, 0.f};
	CVec2 Size = {1.f, 1.f};
	CVec4 Color = { 1.f, 1.f, 1.f, 1.f };
	float Rotation = 0.f;
};

struct TexRenderable : public RenderableInfo
{
	Ref<Texture2D> Texture;
	float TilingFactor = 1.0f;
};

struct SubTexRenderable : public RenderableInfo
{
	Ref<SubTexture2D> SubTexture;
	float TilingFactor = 1.0f;
};



class TextRenderer
{
public:
	static void SetFont(Font* font);
	static void RenderText(const std::string& text, const glm::vec2& position, const glm::vec2& spacing = glm::vec2(0.0f), const glm::vec2& scale = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f));
};

	class Renderer2D
	{
	public:
		static void Init(float Width, float Height);
		

		static void Shutdown();

		static Ref<Texture2D>& GetWhiteTexture();
		static Ref<Texture2D> GetLastTexture();
		static u32 CreateLayer();
		static void UseLayer(u32 LayerToUse);
		static void BeginScene(const Camera& camera, const CMat4& transform);
		static void BeginScene(EditorCamera& camera);
		static void EndScene();
		static void Flush();
		static void Flush(u32 layer);
		static void FlushLines();

		static void DrawScreen();
		static void DrawScreen(const Ref<Texture2D>& Texture);
		// Primitives
		static void Clear(CVec4 color = CVec4(0.f));
		static void DrawLine(const CVec2& p1, const CVec2& p2, float thickness, const CVec4& color);
		static void DrawLine(const CVec2& p1, const CVec2& p2, float thickness, const Ref<Texture2D>& texture, const CVec4& tintColor = CVec4(1.f));

		static void DrawQuad(const CVec2& position, const CVec2& size, const CVec4& color);
		static void DrawQuad(const CVec3& position, const CVec2& size, const CVec4& color);
		static void DrawQuad(const CVec2& position, const CVec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const CVec4& tintColor = CVec4(1.0f));
		static void DrawQuad(const CVec3& position, const CVec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const CVec4& tintColor = CVec4(1.0f));
		static void DrawQuad(const CVec2& position, const CVec2& size, const Ref<SubTexture2D>& texture, float tilingFactor = 1.0f, const CVec4& tintColor = CVec4(1.0f));

		static void DrawQuad(const RenderableInfo& RenderInfo);
		static void DrawQuad(const TexRenderable& RenderInfo);
		static void DrawQuad(const SubTexRenderable& RenderInfo);

		static void DrawQuad(const CMat4& transform, const CVec4& color, const EntityID entityID);
		static void DrawQuad(const CMat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const CVec4& tintColor = CVec4(1.0f));
		static void DrawQuad(const CMat4& transform, const Ref<Texture2D>& texture, const CVec2 TexCoords[4], float tilingFactor = 1.0f, const CVec4& tintColor = CVec4(1.0f));

		static void DrawRotatedQuad(const CVec2& position, const CVec2& size, float rotation, const CVec4& color);
		static void DrawRotatedQuad(const CVec3& position, const CVec2& size, float rotation, const CVec4& color);
		static void DrawRotatedQuad(const CVec2& position, const CVec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const CVec4& tintColor = CVec4(1.0f));
		static void DrawRotatedQuad(const CVec3& position, const CVec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const CVec4& tintColor = CVec4(1.0f));
		static void DrawRotatedQuad(const CVec3& position, const CVec2& size, float rotation, const Ref<SubTexture2D>& texture, float tilingFactor = 1.0f, const CVec4& tintColor = CVec4(1.0f));

		// Stats
		struct Statistics
		{
			u32 DrawCalls = 0;
			u32 QuadCount = 0;

			u32 GetTotalVertexCount() const { return QuadCount * 4; }
			u32 GetTotalIndexCount() const { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();

		//Constants
		struct Constants
		{
			static const u32 MaxQuads = 20000;
			static const u32 MaxVertices = MaxQuads * 4;
			static const u32 MaxIndices = MaxQuads * 6;
			static const u32 MaxTextureSlots = 32; // TODO: RenderCaps
		};

	private:
		static void StartLineBatch();
		static void NextLineBatch();
		static void StartBatch(u32 Layer);
		static void NextBatch(u32 Layer);
	};

#endif