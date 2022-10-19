#include "Clover.h"

#include <Graphics/cube_map.h>
#include <Graphics/renderer.h>
#include <opengl/opengl_cubemap.h>

 
	Ref<CubeMap> CubeMap::Create(const CString& filepath, bool relative)
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::API::none: assert(false);
		case RendererApi::API::OpenGL: return CreateRef<OpenGLCubeMap>(filepath, relative);
		}
		return nullptr;
	}

	Ref<CubeMap> CubeMap::Create(const char* filepaths[])
	{
		switch (RendererApi::GetApi())
		{
		case RendererApi::API::none: assert(false);
		case RendererApi::API::OpenGL: return CreateRef<OpenGLCubeMap>(filepaths);
		}
		return nullptr;
	}


	Ref<CubeMap> CubeMapLibrary::Load(const CString& name, const CString& filepath)
	{
		if (Exists(name))
		{
			Cl_Warning("Texture %s already exists", name.c_str());
			return items_[name];
		}
		Cl_DebugLog("Texture %s Load Successfully", name.c_str());
		Add(name, CubeMap::Create(filepath));

		return items_[name];
	}

	Ref<CubeMap> CubeMapLibrary::Load(const CString& name, const char* filepaths[])
	{
		if (Exists(name))
		{
			Cl_Warning("Texture %s already exists", name.c_str());
			return items_[name];
		}
		Cl_DebugLog("Texture %s Load Successfully", name.c_str());
		Add(name, CubeMap::Create(filepaths));

		return items_[name];
	}
