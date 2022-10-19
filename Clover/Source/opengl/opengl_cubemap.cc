
#include <Clover.h>

#include "opengl.h"
#include "opengl_texture.h"
#include "opengl_cubemap.h"

#include <stb_image.h>
#include <imgui.h>


	static CString newPath;



	void OpenGLCubeMap::setMinFilter(Filter f) {

		glTextureParameteri(render_id, GL_TEXTURE_MIN_FILTER, OpenGLHelper::getGLFilter(f));

	}

	void OpenGLCubeMap::setMagFilter(Filter f) {

		glTextureParameteri(render_id, GL_TEXTURE_MAG_FILTER, OpenGLHelper::getGLFilter(f));

	}

	void OpenGLCubeMap::setWrapS(Wrap c) {

		glTextureParameteri(render_id, GL_TEXTURE_WRAP_S, OpenGLHelper::getGLWrap(c));
	}

	void OpenGLCubeMap::setWrapT(Wrap c) {

		glTextureParameteri(render_id, GL_TEXTURE_WRAP_T, OpenGLHelper::getGLWrap(c));
	}

	void OpenGLCubeMap::setWrapR(Wrap c) {

		glTextureParameteri(render_id, GL_TEXTURE_WRAP_R, OpenGLHelper::getGLWrap(c));
	}

	void OpenGLCubeMap::setWrap(Wrap c) { setWrapS(c); setWrapT(c); setWrapR(c); }

	void OpenGLCubeMap::bind(u32 slot) const
	{
		glBindTextureUnit(slot, render_id);
		glDepthMask(GL_FALSE);
	}

	void OpenGLCubeMap::unBind(u32 slot /*= 0*/) const
	{
		glBindTextureUnit(slot, 0);
		glDepthMask(GL_TRUE);
	}

	void OpenGLCubeMap::ImGuiEditor()
	{

	
	}

	void OpenGLCubeMap::generateMipmaps() const {

		glGenerateTextureMipmap(render_id);
	}

	OpenGLCubeMap::~OpenGLCubeMap()
	{
		glDeleteTextures(1, &render_id);
	}



	void OpenGLCubeMap::Delete()
	{
		glDeleteTextures(1, &render_id);
	}

	OpenGLCubeMap::OpenGLCubeMap(const CString& filepath, bool relative)
	{
		
	}

	OpenGLCubeMap::OpenGLCubeMap(const char* filepaths[])
	{
		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &render_id);


		int w, h, channels;
		for (unsigned int i = 0; i < 6; ++i)
		{
			// note that we store each face with 16 bit floating point values
			stbi_uc* data = stbi_load(filepaths[i], &w, &h, &channels, 0);
			if (data)
			{
				glTextureStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, w, h);
				glTextureSubImage2D(render_id, 0, 0, 0, w, h, GL_RGB, GL_FLOAT, data);
				stbi_image_free(data);
			}

		}
		setWrap(Texture::Wrap::CLAMP_TO_EDGE);
		setMagFilter(Texture::Filter::LINEAR);
		setMinFilter(Texture::Filter::LINEAR);
	}
