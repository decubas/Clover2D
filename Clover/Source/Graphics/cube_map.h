#ifndef _CUBE_MAP_
#define _CUBE_MAP_ 1

#include <Clover.h>

#include <Utils/Library.h>

#include "texture.h"

	// WIP
	class CubeMap : public Texture
	{
	public:

		static Ref<CubeMap> Create(const CString& filepath, bool relative = true);
		static Ref<CubeMap> Create(const char* filepaths[]);
		virtual ~CubeMap() = default;

	};




	// WIP
	class CubeMapLibrary : public Library<CubeMap>
	{
	public:


		Ref<CubeMap> Load(const CString& name, const CString& filepath);
		Ref<CubeMap> Load(const CString& name, const char* filepaths[]);

	};

#endif
