#include "Clover.h"

#include <Graphics/shader.h>
#include <Graphics/renderer.h>

 
	void ShaderLibrary::recompile()
	{
		for (auto& s : items_) 
		{
			s.second->recompile();
		}
	}

	Ref<Shader> ShaderLibrary::Load(const CString& name, const CString& filepath)
	{
		if (Exists(name))
		{
			Cl_Warning("Shader %s already exists", name.c_str());
			return items_[name];
		}
		Cl_DebugLog("Shader %s Load Successfully", name.c_str());
		Add(name, GraphicContext::CreateShader(name, filepath));

		return items_[name];
	}
