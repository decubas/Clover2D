#include "asset.h"
#include "Graphics/texture.h"
#include "Graphics/shader.h"


	///////// LIBS //////////
	static Texture2DLibrary* textureLib_;
	static ShaderLibrary* shaderLib_;
	static Library<Material>* materialLib_;

	static Asset* Assets = new Asset();

	Asset::Asset()
	{
		
		//ModelLibrary* modelLib_ = new Coffee::ModelLibrary();
	}

	Asset::~Asset()
	{
		delete shaderLib_;
		delete materialLib_;
		delete textureLib_;
	}

	void Asset::Initialize()
	{
		textureLib_ = new Texture2DLibrary();
		shaderLib_ = new ShaderLibrary();
		materialLib_ = new Library<Material>();
	}

	Texture2DLibrary& Asset::Textures()
	{
		return *textureLib_;
	}

	ShaderLibrary& Asset::Shaders()
	{
		return *shaderLib_;
	}

	Library<Material>& Asset::Materials()
	{
		return *materialLib_;

	}
