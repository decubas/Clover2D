#include "Clover.h"

#include <Graphics/texture.h>
#include <Graphics/renderer.h>
#include <opengl/opengl_texture.h>


	Texture2DLibrary::Texture2DLibrary()
	{
		u32 whiteTex = 0xffffffff;
		u32 blackTex = 0x00000000;
		u32 errorTex = 0xff00ff;

		Texture::TextureProps props;
		props.format = Texture::Format::RGBA16F;

		defaultWhite = GraphicContext::CreateTexture2D(1, 1, props);
		defaultWhite->setData(&whiteTex, sizeof(u32));

		defaultBlack = GraphicContext::CreateTexture2D(1, 1, props);
		defaultBlack->setData(&blackTex, sizeof(u32));

		defaultError = GraphicContext::CreateTexture2D(1, 1, props);
		defaultError->setData(&errorTex, sizeof(u32));
	}

	Ref<Texture2D> Texture2DLibrary::Load(const CString& name, const CString& filepath)
	{
		if (Exists(name))
		{
			Cl_Warning("Texture {0} already exists", name);
			return items_[name];
		}
		Cl_DebugLog("Texture {0} Load Successfully", name);
		Add(name, GraphicContext::CreateTexture2D(filepath));

		return items_[name];
	}

	


	Ref<Texture2D> Texture2DLibrary::DefaultWhite()
	{
		return defaultWhite;
	}


	Ref<Texture2D> Texture2DLibrary::DefaultBlack()
	{
		return defaultBlack;
	}


	Ref<Texture2D> Texture2DLibrary::DefaultError()
	{
		return defaultError;
	}

	SubTexture2D::SubTexture2D(const Ref<Texture2D> Tex, const glm::vec2& Coords, const glm::vec2& CellSize, const glm::vec2& SpriteSize)
		: m_Texture(Tex)
	{
		m_TexCoords[0] = { (Coords.x * CellSize.x) / Tex->GetWidth(), (Coords.y * CellSize.y) / Tex->GetHeight() };
		m_TexCoords[1] = { ((Coords.x + SpriteSize.x) * CellSize.x) / Tex->GetWidth(), (Coords.y * CellSize.y) / Tex->GetHeight() };
		m_TexCoords[2] = { ((Coords.x + SpriteSize.x) * CellSize.x) / Tex->GetWidth(), ((Coords.y + SpriteSize.y) * CellSize.y) / Tex->GetHeight() };
		m_TexCoords[3] = { (Coords.x * CellSize.x) / Tex->GetWidth(), ((Coords.y + SpriteSize.y) * CellSize.y) / Tex->GetHeight() };
	}



	
