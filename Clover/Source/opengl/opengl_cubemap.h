#ifndef _OPENGL_CUBEMAP__
#define _OPENGL_CUBEMAP__ 1

#include "Graphics/cube_map.h"



	// NOT FINISHED WIP
	class OpenGLCubeMap : public CubeMap {
	public:
		OpenGLCubeMap(const CString& filepath, bool relative = true);
		OpenGLCubeMap(const char* filepaths[]);

		void bind(u32 slot = 0) const override;
		void unBind(u32 slot = 0) const override;

		void ImGuiEditor() override;

		void setMinFilter(Filter f) override;
		void setMagFilter(Filter f) override;
		void setWrapS(Wrap c) override;
		void setWrapT(Wrap c) override;
		void setWrapR(Wrap c) override;
		void setWrap(Wrap c) override;
		void generateMipmaps() const override;


		virtual void setData(const void* data, u32 size) override {}
		virtual void resetTexture(const CString& filepath, bool relative) override {}


		virtual ~OpenGLCubeMap();




		void Delete() override;

	};
#endif
