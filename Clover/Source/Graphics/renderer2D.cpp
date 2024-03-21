#include "Clover.h"

#include <Graphics/renderer2D.h>
#include <Graphics/render_command.h>
#include <Graphics/graphiccontext.h>
#include "Sandbox/rectangle.h"


	struct ScreenVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
	};

	struct Renderer2DScreenData
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		ScreenVertex QuadVertexBufferInfo[4];
		glm::vec4 QuadVertexPositions[4];
	};

	
	struct DefaultRenderData
	{
		Ref<FrameBuffer> ScreenFrameBuf;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		glm::vec4 QuadVertexPositions[4];
	};

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		int TexIndex;
		float TilingFactor;
		int ObjectID = 0;
	};



	struct Renderer2DData
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		glm::mat4 ViewProjection;

		SceneCamera* CurrentCamera;

		std::array<Ref<Texture2D>, 32> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture
	};
	
	struct DefaultTextRenderData
	{
		Font* font = nullptr;
	};

	struct RenderLayer
	{
		Renderer2DData s_Data;
	};

	static Renderer2D::Statistics s_Stats;

	static Renderer2DScreenData s_ScreenData;
	static Renderer2DScreenData s_ScreenRenderData;

	static DefaultRenderData s_DefaultData;
	static DefaultTextRenderData s_DefaultTextData;

	static Renderer2DData s_LineData;
	static Renderer2DData s_LineRenderData;

	static u32 s_CurrentLayer;
	static std::vector<RenderLayer> s_Layers;
	static std::vector<RenderLayer> s_RenderLayers;


	void Renderer2D::Init(float Width, float Height)
	{

		///// SCREEN DATA
#pragma region ScreenData
		s_ScreenData.QuadVertexArray = GraphicContext::CreateVertexArray();
		s_ScreenData.QuadVertexBuffer = GraphicContext::CreateVertexBuffer(4 * sizeof(QuadVertex));

		s_ScreenData.QuadVertexBuffer->setLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});

		s_ScreenData.QuadVertexArray->addVertexBuffer(s_ScreenData.QuadVertexBuffer);

		uint32_t* screen_quadIndices = new uint32_t[6];

		screen_quadIndices[0] = 0;
		screen_quadIndices[1] = 1;
		screen_quadIndices[2] = 2;

		screen_quadIndices[3] = 2;
		screen_quadIndices[4] = 3;
		screen_quadIndices[5] = 0;

		Ref<IndexBuffer> screen_quadIB = GraphicContext::CreateIndexBuffer(screen_quadIndices, 6);
		s_ScreenData.QuadVertexArray->setIndexBuffer(screen_quadIB);
		delete[] screen_quadIndices;

		std::string VertexShader =
			"#version 450 core\n"
			"layout(location = 0) in vec3 a_Position;\n"
			"layout(location = 1) in vec2 a_TexCoord;\n"
			"out vec2 v_TexCoord;\n"
			"void main()\n"
			"{\n"
			"	v_TexCoord = a_TexCoord;\n"
			"	gl_Position = vec4(a_Position, 1.0f);\n"
			"}";


		std::string FragShader = 
			"#version 450 core\n"
			"layout(location = 0) out vec4 color;\n"
			"in vec2 v_TexCoord;\n"
			"uniform sampler2D u_ScreenTexture;\n"
			//"uniform sampler2D u_LastTexture;\n"
			// + texture(u_LastTexture, v_TexCoord)
			"void main()\n"
			"{\n"
			"	color = texture(u_ScreenTexture, v_TexCoord);\n"
			"}\n"
			;

		s_ScreenData.TextureShader = GraphicContext::CreateShader("Screen", FragShader, VertexShader);

		s_ScreenData.QuadVertexBufferInfo[0].Position = { -1.0f, -1.0f, 0.0f };
		s_ScreenData.QuadVertexBufferInfo[1].Position = {  1.0f, -1.0f, 0.0f };
		s_ScreenData.QuadVertexBufferInfo[2].Position = {  1.0f,  1.0f, 0.0f };
		s_ScreenData.QuadVertexBufferInfo[3].Position = { -1.0f,  1.0f, 0.0f };

		s_ScreenData.QuadVertexBufferInfo[0].TexCoord = { 0.0f, 0.0f };
		s_ScreenData.QuadVertexBufferInfo[1].TexCoord = { 1.0f, 0.0f };
		s_ScreenData.QuadVertexBufferInfo[2].TexCoord = { 1.0f, 1.0f };
		s_ScreenData.QuadVertexBufferInfo[3].TexCoord = { 0.0f, 1.0f };

		uint32_t dataSize = (uint32_t)(4 * sizeof(ScreenVertex));

		s_ScreenData.QuadVertexBuffer->uploadData(s_ScreenData.QuadVertexBufferInfo, dataSize);


		s_DefaultData.ScreenFrameBuf = GraphicContext::CreateFrameBuffer(FramebufferSpecification(Width, Height));
#pragma endregion

		////////////////////////////////////// DATA

		Texture::TextureProps props;
		props.format = Texture::Format::RGBA8;
		s_DefaultData.WhiteTexture = GraphicContext::CreateTexture2D(1, 1, props);
		uint32_t whiteTextureData = 0xffffffff;
		s_DefaultData.WhiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[Renderer2D::Constants::MaxTextureSlots];
		for (uint32_t i = 0; i < Renderer2D::Constants::MaxTextureSlots; i++)
			samplers[i] = i;

		VertexShader = "#version 450 core\n"
			"layout(location = 0) in vec3 a_Position;\n"
			"layout(location = 1) in vec4 a_Color;\n"
			"layout(location = 2) in vec2 a_TexCoord;\n"
			"layout(location = 3) in int a_TexIndex;\n"
			"layout(location = 4) in float a_TilingFactor;\n"
			"layout(location = 5) in int a_ObjectID;\n"
			"uniform mat4 u_ViewProjection;\n"
			"out vec4 v_Color;\n"
			"out vec2 v_TexCoord;\n"
			"out int v_TexIndex;\n"
			"out float v_TilingFactor;\n"
			"out flat int v_ObjectID;\n"
			"void main()\n"
			"{\n"
			"	v_Color = a_Color;\n"
			"	v_TexCoord = a_TexCoord;\n"
			"	v_TexIndex = a_TexIndex;\n"
			"	v_TilingFactor = a_TilingFactor;\n"
			"	v_ObjectID = a_ObjectID;\n"
			"	gl_Position =u_ViewProjection* vec4(a_Position, 1.0);\n"
			"}";

		FragShader =
			"#version 450 core\n"
			"layout(location = 0) out vec4 color;\n"
			"layout(location = 1) out int o_IDBuffer;\n"
			"in vec4 v_Color;\n"
			"in vec2 v_TexCoord;\n"
			"in flat int v_TexIndex;\n"
			"in flat float v_TilingFactor;\n"
			"in flat int v_ObjectID;\n"
			"uniform sampler2D u_Textures[32];\n"
			"void main()\n"
			"{\n"
			"	vec4 texColor = v_Color;\n"
			"	switch(int(v_TexIndex))\n"
			"{\n"
			"case 0: texColor *= texture(u_Textures[0], v_TexCoord * v_TilingFactor); break;\n"
			"case 1: texColor *= texture(u_Textures[1], v_TexCoord * v_TilingFactor); break;\n"
			"case 2: texColor *= texture(u_Textures[2], v_TexCoord * v_TilingFactor); break;\n"
			"case 3: texColor *= texture(u_Textures[3], v_TexCoord * v_TilingFactor); break;\n"
			"case 4: texColor *= texture(u_Textures[4], v_TexCoord * v_TilingFactor); break;\n"
			"case 5: texColor *= texture(u_Textures[5], v_TexCoord * v_TilingFactor); break;\n"
			"case 6: texColor *= texture(u_Textures[6], v_TexCoord * v_TilingFactor); break;\n"
			"case 7: texColor *= texture(u_Textures[7], v_TexCoord * v_TilingFactor); break;\n"
			"case 8: texColor *= texture(u_Textures[8], v_TexCoord * v_TilingFactor); break;\n"
			"case 9: texColor *= texture(u_Textures[9], v_TexCoord * v_TilingFactor); break;\n"
			"case 10: texColor *= texture(u_Textures[10], v_TexCoord * v_TilingFactor); break;\n"
			"case 11: texColor *= texture(u_Textures[11], v_TexCoord * v_TilingFactor); break;\n"
			"case 12: texColor *= texture(u_Textures[12], v_TexCoord * v_TilingFactor); break;\n"
			"case 13: texColor *= texture(u_Textures[13], v_TexCoord * v_TilingFactor); break;\n"
			"case 14: texColor *= texture(u_Textures[14], v_TexCoord * v_TilingFactor); break;\n"
			"case 15: texColor *= texture(u_Textures[15], v_TexCoord * v_TilingFactor); break;\n"
			"case 16: texColor *= texture(u_Textures[16], v_TexCoord * v_TilingFactor); break;\n"
			"case 17: texColor *= texture(u_Textures[17], v_TexCoord * v_TilingFactor); break;\n"
			"case 18: texColor *= texture(u_Textures[18], v_TexCoord * v_TilingFactor); break;\n"
			"case 19: texColor *= texture(u_Textures[19], v_TexCoord * v_TilingFactor); break;\n"
			"case 20: texColor *= texture(u_Textures[20], v_TexCoord * v_TilingFactor); break;\n"
			"case 21: texColor *= texture(u_Textures[21], v_TexCoord * v_TilingFactor); break;\n"
			"case 22: texColor *= texture(u_Textures[22], v_TexCoord * v_TilingFactor); break;\n"
			"case 23: texColor *= texture(u_Textures[23], v_TexCoord * v_TilingFactor); break;\n"
			"case 24: texColor *= texture(u_Textures[24], v_TexCoord * v_TilingFactor); break;\n"
			"case 25: texColor *= texture(u_Textures[25], v_TexCoord * v_TilingFactor); break;\n"
			"case 26: texColor *= texture(u_Textures[26], v_TexCoord * v_TilingFactor); break;\n"
			"case 27: texColor *= texture(u_Textures[27], v_TexCoord * v_TilingFactor); break;\n"
			"case 28: texColor *= texture(u_Textures[28], v_TexCoord * v_TilingFactor); break;\n"
			"case 29: texColor *= texture(u_Textures[29], v_TexCoord * v_TilingFactor); break;\n"
			"case 30: texColor *= texture(u_Textures[30], v_TexCoord * v_TilingFactor); break;\n"
			"case 31: texColor *= texture(u_Textures[31], v_TexCoord * v_TilingFactor); break;\n"
			"}\n"
			"	color = texColor;\n"
			"	o_IDBuffer = v_ObjectID;\n"
			"}\n";

		s_DefaultData.TextureShader = GraphicContext::CreateShader("SimpleTexture", FragShader, VertexShader);
		s_DefaultData.TextureShader->bind();
		s_DefaultData.TextureShader->setIntArray("u_Textures", samplers, Renderer2D::Constants::MaxTextureSlots);


		s_DefaultData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_DefaultData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_DefaultData.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_DefaultData.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

#pragma region Start Line Data
		s_LineData.TextureSlots[0] = s_DefaultData.WhiteTexture;
		s_LineData.QuadVertexBufferBase = new QuadVertex[Renderer2D::Constants::MaxVertices];

		s_LineData.QuadVertexArray = GraphicContext::CreateVertexArray();
		s_LineData.QuadVertexBuffer = GraphicContext::CreateVertexBuffer(Renderer2D::Constants::MaxVertices * sizeof(QuadVertex));

		s_LineData.QuadVertexBuffer->setLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Int, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" },
			{ ShaderDataType::Int,   "a_ObjectID" }
			});


		s_LineData.QuadVertexArray->addVertexBuffer(s_LineData.QuadVertexBuffer);


		uint32_t* quadIndices = new uint32_t[Renderer2D::Constants::MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < Renderer2D::Constants::MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		Ref<IndexBuffer> quadIB = GraphicContext::CreateIndexBuffer(quadIndices, Renderer2D::Constants::MaxIndices);

		s_LineData.QuadVertexArray->setIndexBuffer(quadIB);
		delete[] quadIndices;

		StartLineBatch();
#pragma endregion
	}

	void Renderer2D::Shutdown()
	{
		for (u32 i = 0; i < s_Layers.size(); ++i)
		{
			if (s_Layers[i].s_Data.QuadVertexBufferBase)
			{
				delete[] s_Layers[i].s_Data.QuadVertexBufferBase;
				s_Layers[i].s_Data.QuadVertexBufferBase = nullptr;
			}
			s_Layers[i].s_Data.QuadVertexArray.reset();
		}	

		s_DefaultData.ScreenFrameBuf.reset();
		s_DefaultData.TextureShader.reset();
		s_DefaultData.WhiteTexture.reset();
	}

	Ref<Texture2D>& Renderer2D::GetWhiteTexture()
	{
		return s_DefaultData.WhiteTexture;
	}

	Ref<Texture2D> Renderer2D::GetLastTexture()
	{
		return s_DefaultData.ScreenFrameBuf->getColorTexture();
	}

	u32 Renderer2D::CreateLayer()
	{
		RenderLayer NewLayer;

		// Set first texture slot to 0
		 NewLayer.s_Data.TextureSlots[0] = s_DefaultData.WhiteTexture;
		 NewLayer.s_Data.QuadVertexBufferBase = new QuadVertex[Renderer2D::Constants::MaxVertices];

		 NewLayer.s_Data.QuadVertexArray = GraphicContext::CreateVertexArray();
		 NewLayer.s_Data.QuadVertexBuffer = GraphicContext::CreateVertexBuffer(Renderer2D::Constants::MaxVertices * sizeof(QuadVertex));

		 NewLayer.s_Data.QuadVertexBuffer->setLayout({
			 { ShaderDataType::Float3, "a_Position" },
			 { ShaderDataType::Float4, "a_Color" },
			 { ShaderDataType::Float2, "a_TexCoord" },
			 { ShaderDataType::Int, "a_TexIndex" },
			 { ShaderDataType::Float, "a_TilingFactor" },
			 { ShaderDataType::Int,   "a_ObjectID" }
			 });


		 NewLayer.s_Data.QuadVertexArray->addVertexBuffer(NewLayer.s_Data.QuadVertexBuffer);


		 uint32_t* quadIndices = new uint32_t[Renderer2D::Constants::MaxIndices];

		 uint32_t offset = 0;
		 for (uint32_t i = 0; i < Renderer2D::Constants::MaxIndices; i += 6)
		 {
			 quadIndices[i + 0] = offset + 0;
			 quadIndices[i + 1] = offset + 1;
			 quadIndices[i + 2] = offset + 2;

			 quadIndices[i + 3] = offset + 2;
			 quadIndices[i + 4] = offset + 3;
			 quadIndices[i + 5] = offset + 0;

			 offset += 4;
		 }
		 Ref<IndexBuffer> quadIB = GraphicContext::CreateIndexBuffer(quadIndices, Renderer2D::Constants::MaxIndices);

		 NewLayer.s_Data.QuadVertexArray->setIndexBuffer(quadIB);
		 delete[] quadIndices;

		 NewLayer.s_Data.QuadIndexCount = 0;
		 NewLayer.s_Data.QuadVertexBufferPtr = NewLayer.s_Data.QuadVertexBufferBase;
		 NewLayer.s_Data.TextureSlotIndex = 1;

		s_Layers.push_back(NewLayer);
		s_RenderLayers.push_back(NewLayer);

		return s_Layers.size() - 1;
	}

	void Renderer2D::BeginScene(const Camera& camera, const CMat4& transform)
	{
		s_Layers[s_CurrentLayer].s_Data.ViewProjection = camera.GetProjection() * transform;
		s_DefaultData.ScreenFrameBuf->bind();

		StartLineBatch();
	}

	void Renderer2D::BeginScene(EditorCamera& camera)
	{
		s_Layers[s_CurrentLayer].s_Data.CurrentCamera = (SceneCamera*) & camera;
		s_Layers[s_CurrentLayer].s_Data.ViewProjection = camera.GetProjection() * camera.GetTransform().RecalculateModel();
		s_DefaultData.ScreenFrameBuf->bind();

		StartLineBatch();
	}

	void Renderer2D::UseLayer(u32 LayerToUse)
	{
		s_CurrentLayer = LayerToUse;
	}

	void Renderer2D::EndScene()
	{
		Flush();
		FlushLines();
		s_DefaultData.ScreenFrameBuf->unBind();

		Renderer2D::DrawScreen();
	}

	void Renderer2D::StartLineBatch()
	{
		s_LineData.QuadIndexCount = 0;
		s_LineData.QuadVertexBufferPtr = s_LineData.QuadVertexBufferBase;
		s_LineData.TextureSlotIndex = 1;
	}

	void Renderer2D::NextLineBatch()
	{
		FlushLines();
		StartLineBatch();
	}

	void Renderer2D::StartBatch(u32 Layer)
	{
		s_Layers[Layer].s_Data.QuadIndexCount = 0;
		s_Layers[Layer].s_Data.QuadVertexBufferPtr = s_Layers[Layer].s_Data.QuadVertexBufferBase;
		s_Layers[Layer].s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::Flush()
	{
		for (u32 Index = 0; Index < s_Layers.size(); ++Index)
		{
			Flush(Index);
		}
	}

	void Renderer2D::Flush(u32 layer)
	{
		if (s_Layers[layer].s_Data.QuadIndexCount == 0)
			return; // Nothing to draw

		s_RenderLayers[layer] = s_Layers[layer];
		u32 CurrentLayer = layer;
		Renderer::Submit([CurrentLayer]() {
			s_DefaultData.TextureShader->bind();
			s_DefaultData.TextureShader->setMat4("u_ViewProjection", s_Layers[CurrentLayer].s_Data.ViewProjection);

			//uint32_t dataSize = (uint32_t)(s_RenderData.QuadIndexCount * sizeof(QuadVertex));
			uint32_t dataSize = (uint32_t)((uint8_t*)s_RenderLayers[CurrentLayer].s_Data.QuadVertexBufferPtr - (uint8_t*)s_RenderLayers[CurrentLayer].s_Data.QuadVertexBufferBase);
			s_RenderLayers[CurrentLayer].s_Data.QuadVertexBuffer->uploadData(s_RenderLayers[CurrentLayer].s_Data.QuadVertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < s_RenderLayers[CurrentLayer].s_Data.TextureSlotIndex; i++)
				s_RenderLayers[CurrentLayer].s_Data.TextureSlots[i]->bind(i);

			RendererCommand::DrawIndexed(s_RenderLayers[CurrentLayer].s_Data.QuadVertexArray, s_RenderLayers[CurrentLayer].s_Data.QuadIndexCount);
			});

		s_Stats.DrawCalls++;

		StartBatch(layer);
	}

	void Renderer2D::FlushLines()
	{
		if (s_LineData.QuadIndexCount == 0)
			return; // Nothing to draw

		s_LineRenderData = s_LineData;
		Renderer::Submit([]() {
			s_DefaultData.TextureShader->bind();
			s_DefaultData.TextureShader->setMat4("u_ViewProjection", s_Layers[0].s_Data.ViewProjection);

			//uint32_t dataSize = (uint32_t)(s_RenderData.QuadIndexCount * sizeof(QuadVertex));
			uint32_t dataSize = (uint32_t)((uint8_t*)s_LineRenderData.QuadVertexBufferPtr - (uint8_t*)s_LineRenderData.QuadVertexBufferBase);
			s_LineRenderData.QuadVertexBuffer->uploadData(s_LineRenderData.QuadVertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < s_LineRenderData.TextureSlotIndex; i++)
				s_LineRenderData.TextureSlots[i]->bind(i);

			RendererCommand::DrawIndexed(s_LineRenderData.QuadVertexArray, s_LineRenderData.QuadIndexCount);
			});

		s_Stats.DrawCalls++;

		StartLineBatch();
	}

	void Renderer2D::DrawScreen(const Ref<Texture2D>& Texture)
	{
		s_ScreenRenderData = s_ScreenData;
 		Renderer::Submit([Texture]() {
			s_ScreenRenderData.TextureShader->bind();
			s_ScreenRenderData.TextureShader->setInt("u_ScreenTexture", 0);

			s_ScreenRenderData.QuadVertexBuffer->bind();

			// Bind textures
			Texture->bind();

			RendererCommand::DrawIndexed(s_ScreenRenderData.QuadVertexArray, 6);
			});
	}

	void Renderer2D::DrawScreen()
	{
		DrawScreen(s_DefaultData.ScreenFrameBuf->getColorTexture());
	}

	void Renderer2D::Clear(CVec4 color /*= CVec4(0.f)*/)
	{
		Renderer::Submit([color]() {
			RendererCommand::clearColor(color);
			RendererCommand::Clear();
			});
	}

	void Renderer2D::NextBatch(u32 Layer)
	{
		Flush(Layer);
		StartBatch(Layer);
	}

	void Renderer2D::DrawLine(const CVec2& p1, const CVec2& p2, float thickness, const CVec4& color)
	{
		DrawLine(p1, p2, thickness, s_DefaultData.WhiteTexture, color);
	}

	void Renderer2D::DrawLine(const CVec2& p1, const CVec2& p2, float thickness, const Ref<Texture2D>& texture, const CVec4& tintColor /*= CVec4(1.f)*/)
	{

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_LineData.QuadIndexCount >= Renderer2D::Constants::MaxIndices)
			NextLineBatch();

		int textureIndex = 0;
		for (uint32_t i = 1; i < s_LineData.TextureSlotIndex; i++)
		{
			if (*s_LineData.TextureSlots[i] == *texture)
			{
				textureIndex = (int)i;
				break;
			}
		}

		if (textureIndex == 0)
		{
			if (s_LineData.TextureSlotIndex >= Renderer2D::Constants::MaxTextureSlots)
				NextLineBatch();

			textureIndex = (int)s_LineData.TextureSlotIndex;
			s_LineData.TextureSlots[s_LineData.TextureSlotIndex] = texture;
			s_LineData.TextureSlotIndex++;
		}
		glm::vec3 direction = glm::vec3{ p2.x, p2.y, 0.f } - glm::vec3{ p1.x, p1.y, 0.f };
		direction = glm::normalize(direction);
		glm::mat4 Rotation = glm::rotate(glm::mat4(1.0f),
			(atan2(-direction.y, direction.x) - 1.5708f),
			{ 0.0f, 0.0f, -1.0f });

		glm::mat4 Scale = glm::scale(glm::mat4(1.0f), { thickness, 0.0f, 0.0f });
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3{ p1.x , p1.y, 0.f }) * Rotation;
		s_LineData.QuadVertexBufferPtr->Position = transform * glm::vec4{ -thickness , 0, 0, 1 }; //s_DefaultData.QuadVertexPositions[0];
		s_LineData.QuadVertexBufferPtr->Color = tintColor;
		s_LineData.QuadVertexBufferPtr->TexCoord = textureCoords[0];
		s_LineData.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_LineData.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_LineData.QuadVertexBufferPtr++;

		s_LineData.QuadVertexBufferPtr->Position = transform * glm::vec4{ thickness , 0, 0, 1 }; //s_DefaultData.QuadVertexPositions[1];
		s_LineData.QuadVertexBufferPtr->Color = tintColor;
		s_LineData.QuadVertexBufferPtr->TexCoord = textureCoords[1];
		s_LineData.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_LineData.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_LineData.QuadVertexBufferPtr++;

		transform = glm::translate(glm::mat4(1.0f), glm::vec3{ p2.x , p2.y, 0.f }) * Rotation;
		s_LineData.QuadVertexBufferPtr->Position = transform * glm::vec4{ thickness , 0, 0, 1 }; //s_DefaultData.QuadVertexPositions[2];
		s_LineData.QuadVertexBufferPtr->Color = tintColor;
		s_LineData.QuadVertexBufferPtr->TexCoord = textureCoords[2];
		s_LineData.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_LineData.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_LineData.QuadVertexBufferPtr++;

		s_LineData.QuadVertexBufferPtr->Position = transform * glm::vec4{ -thickness , 0, 0, 1 }; //s_DefaultData.QuadVertexPositions[3];
		s_LineData.QuadVertexBufferPtr->Color = tintColor;
		s_LineData.QuadVertexBufferPtr->TexCoord = textureCoords[3];
		s_LineData.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_LineData.QuadVertexBufferPtr->TilingFactor = 1.0f;
		s_LineData.QuadVertexBufferPtr++;


		s_LineData.QuadIndexCount += 6;

// 		for (size_t i = 0; i < quadVertexCount; i++)
// 		{
// 			s_LineData.QuadVertexBufferPtr->Position = transform * s_DefaultData.QuadVertexPositions[i];
// 			s_LineData.QuadVertexBufferPtr->Color = tintColor;
// 			s_LineData.QuadVertexBufferPtr->TexCoord = textureCoords[i];
// 			s_LineData.QuadVertexBufferPtr->TexIndex = textureIndex;
// 			s_LineData.QuadVertexBufferPtr->TilingFactor = 1.0f;
// 			s_LineData.QuadVertexBufferPtr++;
// 		}

		s_Stats.QuadCount++;
	}

	void Renderer2D::DrawRect(const CVec2& position, const CVec2& size, float thickness, const CVec4& color)
	{
		const CVec2 Pos = position - size * 0.5f;
		DrawLine(Pos, {Pos.x + size.x, Pos.y}, thickness, color);
		DrawLine({ Pos.x + size.x, Pos.y }, { Pos.x + size.x, Pos.y + size.y }, thickness, color);
		DrawLine({ Pos.x + size.x, Pos.y + size.y }, { Pos.x, Pos.y + size.y }, thickness, color);
		DrawLine(Pos, { Pos.x + size.x, Pos.y }, thickness, color);
		DrawLine({ Pos.x, Pos.y + size.y }, Pos, thickness, color);
	}

	void Renderer2D::DrawQuad(const CVec2& position, const CVec2& size, const CVec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const CVec3& position, const CVec2& size, const CVec4& color)
	{

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color, 0);
	}

	void Renderer2D::DrawQuad(const CVec2& position, const CVec2& size, const Ref<Texture2D>& texture, float tilingFactor, const CVec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const CVec3& position, const CVec2& size, const Ref<Texture2D>& texture, float tilingFactor, const CVec4& tintColor)
	{
		Box2D AABB = Box2D(position.x - size.x * 0.5f, position.y - size.y * 0.5f, size.x, size.y);
		if (!s_Layers[s_CurrentLayer].s_Data.CurrentCamera->IsInFrustum(AABB))
		{
			//return;
		}


		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const CMat4& transform, const CVec4& color, const EntityID entityID)
	{

		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (s_Layers[s_CurrentLayer].s_Data.QuadIndexCount >= Renderer2D::Constants::MaxIndices)
			NextBatch(s_CurrentLayer);

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Layers[s_CurrentLayer].s_Data.QuadVertexBufferPtr->Position = transform * s_DefaultData.QuadVertexPositions[i];
			s_Layers[s_CurrentLayer].s_Data.QuadVertexBufferPtr->Color = color;
			s_Layers[s_CurrentLayer].s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Layers[s_CurrentLayer].s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Layers[s_CurrentLayer].s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Layers[s_CurrentLayer].s_Data.QuadVertexBufferPtr->ObjectID = (int)entityID;
			s_Layers[s_CurrentLayer].s_Data.QuadVertexBufferPtr++;
		}

		s_Layers[s_CurrentLayer].s_Data.QuadIndexCount += 6;

		s_Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const CMat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const CVec4& tintColor)
	{
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		

		DrawQuad(transform, texture, textureCoords, tilingFactor, tintColor);
/*
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.QuadIndexCount >= Renderer2D::Constants::MaxIndices)
			NextBatch();

		int textureIndex = 0;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (int)i;
				break;
			}
		}

		if (textureIndex == 0)
		{
			if (s_Data.TextureSlotIndex >= Renderer2D::Constants::MaxTextureSlots)
				NextBatch();

			textureIndex = (int)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Stats.QuadCount++;*/
	}

	void Renderer2D::DrawQuad(const CVec2& position, const CVec2& size, const Ref<SubTexture2D>& texture, float tilingFactor /*= 1.0f*/, const CVec4& tintColor /*= CVec4(1.0f)*/)
	{
		Box2D AABB = Box2D(position.x - size.x * 0.5f, position.y - size.y * 0.5f, size.x, size.y);
		if (!s_Layers[s_CurrentLayer].s_Data.CurrentCamera->IsInFrustum(AABB))
		{
			return;
		}


		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture->GetTexture(), texture->GetTexCoords(), tilingFactor, tintColor);
	}

	void Renderer2D::DrawQuad(const CMat4& transform, const Ref<Texture2D>& texture, const CVec2 TexCoords[4], float tilingFactor /*= 1.0f*/, const CVec4& tintColor /*= CVec4(1.0f)*/)
	{
		constexpr size_t quadVertexCount = 4;

		if (s_Layers[s_CurrentLayer].s_Data.QuadIndexCount >= Renderer2D::Constants::MaxIndices)
			NextBatch(s_CurrentLayer);

		int textureIndex = 0;
		for (uint32_t i = 1; i < s_Layers[s_CurrentLayer].s_Data.TextureSlotIndex; i++)
		{
			if (!s_Layers[s_CurrentLayer].s_Data.TextureSlots[i])
			{
				s_Layers[s_CurrentLayer].s_Data.TextureSlots[i] = s_DefaultData.WhiteTexture;
			}

			if (*s_Layers[s_CurrentLayer].s_Data.TextureSlots[i] == *texture)
			{
				textureIndex = (int)i;
				break;
			}
		}

		if (textureIndex == 0)
		{
			if (s_Layers[s_CurrentLayer].s_Data.TextureSlotIndex >= Renderer2D::Constants::MaxTextureSlots)
				NextBatch(s_CurrentLayer);

			textureIndex = (int)s_Layers[s_CurrentLayer].s_Data.TextureSlotIndex;
			s_Layers[s_CurrentLayer].s_Data.TextureSlots[s_Layers[s_CurrentLayer].s_Data.TextureSlotIndex] = texture;
			s_Layers[s_CurrentLayer].s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Layers[s_CurrentLayer].s_Data.QuadVertexBufferPtr->Position = transform * s_DefaultData.QuadVertexPositions[i];
			s_Layers[s_CurrentLayer].s_Data.QuadVertexBufferPtr->Color = tintColor;
			s_Layers[s_CurrentLayer].s_Data.QuadVertexBufferPtr->TexCoord = TexCoords[i];
			s_Layers[s_CurrentLayer].s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Layers[s_CurrentLayer].s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Layers[s_CurrentLayer].s_Data.QuadVertexBufferPtr++;
		}

		s_Layers[s_CurrentLayer].s_Data.QuadIndexCount += 6;

		s_Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const RenderableInfo& RenderInfo)
	{
		DrawRotatedQuad(RenderInfo.Position, RenderInfo.Size, RenderInfo.Rotation, RenderInfo.Color);
	}

	void Renderer2D::DrawQuad(const TexRenderable& RenderInfo)
	{
		DrawRotatedQuad(CVec3(RenderInfo.Position, 0.f), RenderInfo.Size, RenderInfo.Rotation, RenderInfo.Texture, RenderInfo.TilingFactor, RenderInfo.Color);
	}

	void Renderer2D::DrawQuad(const SubTexRenderable& RenderInfo)
	{
		DrawRotatedQuad(CVec3(RenderInfo.Position, 0.f), RenderInfo.Size, RenderInfo.Rotation, RenderInfo.SubTexture, RenderInfo.TilingFactor, RenderInfo.Color);
	}

	void Renderer2D::DrawRotatedQuad(const CVec2& position, const CVec2& size, float rotation, const CVec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const CVec3& position, const CVec2& size, float rotation, const CVec4& color)
	{

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color, 0);
	}

	void Renderer2D::DrawRotatedQuad(const CVec2& position, const CVec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const CVec4& tintColor)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const CVec3& position, const CVec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const CVec4& tintColor)
	{

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const CVec3& position, const CVec2& size, float rotation, const Ref<SubTexture2D>& texture, float tilingFactor /*= 1.0f*/, const CVec4& tintColor /*= CVec4(1.0f)*/)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture->GetTexture(), texture->GetTexCoords(), tilingFactor, tintColor);
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Stats;
	}


	/*void TextRenderer::Load(const std::string& fontPath, unsigned int fontSize)
	{
		// first clear the previously loaded Characters
		s_DefaultTextData.Characters.clear();

		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
		if (ft) {
			msdfgen::FontHandle* font = msdfgen::loadFont(ft, fontPath.c_str());
			if (font) {
				// then for the first 128 ASCII characters, pre-load/compile their characters and store them
				for (unsigned char c = 0; c < 128; c++)
				{
					msdfgen::Shape shape;
					if (msdfgen::loadGlyph(shape, font, c))
					{
						if (shape.validate() && shape.contours.size() > 0)
						{
							shape.normalize();
							//                      max. angle
							msdfgen::edgeColoringSimple(shape, 3.0);
							//           image width, height
							msdfgen::Bitmap<float, 4> msdf(32, 32);
							//                     range, scale, translation
							msdfgen::generateMTSDF(msdf, shape, 4.0, 1.0, msdfgen::Vector2(0.0, 0.0));
							byte* pixels = new byte[4 * msdf.width() * msdf.height()];
							for (int y = msdf.height() - 1; y >= 0; --y)
							{
								for (int x = 0; x < msdf.width(); ++x)
								{
									*pixels++ = msdfgen::pixelFloatToByte(msdf(x, y)[0]);
									*pixels++ = msdfgen::pixelFloatToByte(msdf(x, y)[1]);
									*pixels++ = msdfgen::pixelFloatToByte(msdf(x, y)[2]);
									*pixels++ = msdfgen::pixelFloatToByte(msdf(x, y)[3]);
								}
							}
							//msdfgen::savePng(msdf, "output.png");
							//msdfgen::simulate8bit(msdf);

							Texture::TextureProps props;
							props.format = Texture::Format::RGBA8;
							props.filter = Texture::Filter::LINEAR;
							props.wrap = Texture::Wrap::CLAMP_TO_EDGE;
							Ref<Texture2D> tex = GraphicContext::CreateTexture2D(msdf.width(), msdf.height(), props);
							tex->setData(pixels, 0);
							delete[] pixels;
							// now store character for later use
							Character character = {
								tex,
								glm::ivec2(msdf.width(), msdf.height()),
								glm::ivec2(shape.getBounds().l, shape.getBounds().t),
								4
							};
							s_DefaultTextData.Characters.insert(std::pair<char, Character>(c, character));
							
						}
					}
				}
				
				msdfgen::destroyFont(font);
			}
			msdfgen::deinitializeFreetype(ft);
		}

		
	}
*/

	void TextRenderer::SetFont(Font* font)
	{
		s_DefaultTextData.font = font;
	}

	void TextRenderer::RenderText(const std::string& text, const glm::vec2& position, const glm::vec2& spacing, const glm::vec2& scale, const glm::vec4& color)
	{
		if (s_DefaultTextData.font == nullptr)
			return;

		float xpos = position.x;
		float ypos = position.y;
		// iterate through all characters
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			
			Character ch = s_DefaultTextData.font->Characters[*c];

			if (!ch.CharTexture)
				ch = s_DefaultTextData.font->Characters[' '];

			float w = ch.Size.x * scale.x;
			float h = ch.Size.y * scale.y;

			if (*c == '\n')
			{
				ypos += h + spacing.y;
				xpos = position.x;
				continue;
			}

			Renderer2D::DrawQuad({ xpos, ypos }, { w, h }, ch.CharTexture, 1, color);

			xpos += w + spacing.x;
		}
	}
