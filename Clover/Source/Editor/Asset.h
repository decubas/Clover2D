#pragma once


#include <Clover.h>

#include <Utils/Library.h>

	class Texture2DLibrary;
	class ShaderLibrary;
	class Texture2D;
	class Shader;
	class Material;



	class Asset
	{
		friend Asset;
	public:
		Asset();
		~Asset();
		static void Initialize();

		static Texture2DLibrary& Textures();
		static ShaderLibrary& Shaders();
		static Library<Material>& Materials();
		
	};
