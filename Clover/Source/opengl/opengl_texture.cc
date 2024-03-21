#include "Clover.h"


#include <Graphics/renderer.h>
#include <opengl/opengl_texture.h>

#include <stb_image.h>
#include <imgui.h> 

 
    void OpenGLTexture2D::setMinFilter(Filter f) {

        glTextureParameteri(render_id, GL_TEXTURE_MIN_FILTER, OpenGLHelper::getGLFilter(f));
        
    }

    void OpenGLTexture2D::setMagFilter(Filter f) {

        glTextureParameteri(render_id, GL_TEXTURE_MAG_FILTER, OpenGLHelper::getGLFilter(f));
        
    }

    void OpenGLTexture2D::setWrapS(Wrap c) {
      
        glTextureParameteri(render_id, GL_TEXTURE_WRAP_S, OpenGLHelper::getGLWrap(c));
    }

    void OpenGLTexture2D::setWrapT(Wrap c) {

        glTextureParameteri(render_id, GL_TEXTURE_WRAP_T, OpenGLHelper::getGLWrap(c));
    }

    void OpenGLTexture2D::setWrapR(Wrap c) {
  
        glTextureParameteri(render_id, GL_TEXTURE_WRAP_R, OpenGLHelper::getGLWrap(c));
    }

    void OpenGLTexture2D::setWrap(Wrap c) { setWrapS(c); setWrapT(c); setWrapR(c); }

    void OpenGLTexture2D::setData(const void* data, u32 size) 
    {
		u32 bpp = 0;
		if (OpenGLHelper::getGlenumFormat(props_.format) == GL_RGBA)
		{
			bpp = 4;
		}
		else
		{
			bpp = 3;
		}

		Cl_Assert(data != nullptr, "Failed to load data!!");
        glTextureSubImage2D(render_id, 0, 0, 0, width_, height_, OpenGLHelper::getGlenumFormat(props_.format), GL_UNSIGNED_BYTE, data);
    }

	void OpenGLTexture2D::resetTexture(const CString& filepath, bool relative)
	{
        if(render_id)glDeleteTextures(1, &render_id);
        CString path;
        
		path = filepath;
        path_ = filepath;
		
        int w, h, channels;
		stbi_uc* data = stbi_load(path.c_str(), &w, &h, &channels, 0);

		if (!data)
		{
			Cl_Error("Failed to load image %s!!", filepath.c_str());
			w = 1;
			h = 1;
			channels = 3;
		} 

		width_ = w;
		height_ = h;

		GLenum internalFormat = GL_RGB8;
		GLenum dataFormat = GL_RGB;

        switch (channels)
        {
		case 4:
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
            props_.format = Texture::Format::RGBA16F;
			break;
		case 3:
			internalFormat = GL_RGB8;
			//internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
            //props_.format = Texture::Format::F_RGB;
			break;
		case 2:
			internalFormat = GL_RG8;
			dataFormat = GL_RG;
           // props_.format = Texture::Format::F_RG;
			break;
		case 1:
			internalFormat = GL_RED;
			dataFormat = GL_RED;
           // props_.format = Texture::Format::F_R;
			break;
        }

		//C_ASSERT(internalFormat & dataFormat, "Texture Format Not Supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &render_id);
		bind();
		glTextureStorage2D(render_id, 1, internalFormat, width_, height_);

        if (data)
        {
            glTextureSubImage2D(render_id, 0, 0, 0, width_, height_, dataFormat, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
		else
		{
			Cl_Error("Failed to load image %s!!", filepath.c_str());
			path_ = "Error Texture";
			u32 errorTex = 0xff00ff;
			setData(&errorTex, sizeof(u32));
		}


		setMinFilter(props_.filter);
		setMagFilter(props_.filter);
		setWrap(props_.wrap);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTextureParameterfv(render_id, GL_TEXTURE_BORDER_COLOR, borderColor);
		unBind();
	}

	void OpenGLTexture2D::bind(u32 slot) const
    {
		glBindTextureUnit(slot, render_id);
    }

	void OpenGLTexture2D::unBind(u32 slot /*= 0*/) const
	{
		glBindTextureUnit(slot, 0);
	}
	static bool InfoOpen = false;

    void OpenGLTexture2D::ImGuiEditor()
    {
        if (ImGui::ImageButton((ImTextureID)
            (ImTextureID)render_id,
            ImVec2(50, 50),
            ImVec2(0, 1),
            ImVec2(1, 0), 0))
        {}

		if (open)
		{
			ImGui::Text(path_.c_str());
			ImGui::Text(shaderName_.c_str());
			if (ImGui::CollapsingHeader("Edit Properties"))
			{
				ImGui::Separator();


				ImGui::Separator();
				if (ImGui::Button("Set Min LINEAR"))
				{
					setMinFilter(Texture::Filter::LINEAR);
				}
				if (ImGui::Button("Set Min NEAREST"))
				{
					setMinFilter(Texture::Filter::NEAREST);
				}

				ImGui::Separator();
				if (ImGui::Button("Set Mag LINEAR"))
				{
					setMagFilter(Texture::Filter::LINEAR);
				}
				if (ImGui::Button("Set Mag NEAREST"))
				{
					setMagFilter(Texture::Filter::NEAREST);
				}

				ImGui::Separator();
				if (ImGui::Button("Set CLAMP TO EDGE"))
				{
					setWrap(Texture::Wrap::CLAMP_TO_EDGE);
				}
				if (ImGui::Button("Set MIRRORED REPEAT"))
				{
					setWrap(Texture::Wrap::MIRRORED_REPEAT);
				}
				if (ImGui::Button("Set REPEAT"))
				{
					setWrap(Texture::Wrap::REPEAT);
				}



				ImGui::Text("Properties");
				switch (props_.format)
				{
					//case Texture::Format::R: ImGui::Text("R"); break;
					//case Texture::Format::RG: ImGui::Text("RG"); break;
					//case Texture::Format::RGB: ImGui::Text("RGB"); break;
				case Texture::Format::RG32F: ImGui::Text("RG32F"); break;
				case Texture::Format::RGBA16F: ImGui::Text("RGBA16F"); break;
				case Texture::Format::DEPTH32F: ImGui::Text("DEPTH32F"); break;
				case Texture::Format::DEPTH24STENCIL8: ImGui::Text("DEPTH24STENCIL8"); break;
				}

				switch (props_.filter)
				{
				case Texture::Filter::LINEAR: ImGui::Text("LINEAR"); break;
				case Texture::Filter::NEAREST: ImGui::Text("NEAREST"); break;
				}

				switch (props_.wrap)
				{
				case Texture::Wrap::CLAMP_TO_EDGE: ImGui::Text("CLAP TO EDGE"); break;
				case Texture::Wrap::MIRRORED_REPEAT: ImGui::Text("MIRRORED REPEAT"); break;
				case Texture::Wrap::REPEAT: ImGui::Text("REPEAT"); break;
				}



			}
		}
    }

	void OpenGLTexture2D::generateMipmaps() const {

        glGenerateTextureMipmap(render_id);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &render_id);
    }



	void OpenGLTexture2D::Delete()
	{
		glDeleteTextures(1, &render_id);
	}

	OpenGLTexture2D::OpenGLTexture2D(const CString& filepath, bool relative)
	{
        resetTexture(filepath, relative);
        generateMipmaps();
	}

	OpenGLTexture2D::OpenGLTexture2D(const u32 width, const u32 height, const TextureProps& properties)
	{
		width_ = width;
		height_ = height;

        path_ = "Texture from memory";
        props_ = properties;
       


		glCreateTextures(GL_TEXTURE_2D, 1, &render_id);
		glBindTexture(GL_TEXTURE_2D, render_id);
        glTextureStorage2D(render_id, 1, OpenGLHelper::getGlenumInternalFormat(props_.format), width_, height_);

   
        setMagFilter(props_.filter);
        setMinFilter(props_.filter);
        setWrap(props_.wrap);
	}
