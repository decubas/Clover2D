#ifndef _OPENGL_TEXTURE__
#define _OPENGL_TEXTURE__ 1

#include "opengl.h"

#include "Graphics/texture.h"



    class OpenGLTexture2D : public Texture2D {
    public:
        OpenGLTexture2D(const CString& filepath, bool relative);
        OpenGLTexture2D(const u32 width, const u32 height, const TextureProps& properties);

        void setData(const void *data, u32 size) override; // The LOD to fill (mipmapping only)

        void resetTexture(const CString& filepath, bool relative = true) override;

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


        virtual ~OpenGLTexture2D();


        void Delete() override;

    private:
		bool open = false;


    };
#endif
