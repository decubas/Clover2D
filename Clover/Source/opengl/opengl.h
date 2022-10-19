#ifndef INCLUDE_OPENGL
#define INCLUDE_OPENGL

#include <Clover.h>
#include "Graphics/texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


	void GLAPIENTRY MessageCallback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam);

// Gets Opengl Errors

	class OpenGLHelper
	{
	public:
		inline static void OpenGLHelper::GLGetError(const char* name)
		{
			GLenum error = glGetError();

			if (error == GL_INVALID_ENUM)
			{
				Cl_Error("%s GL_INVALID_ENUM\n", name);
			}
			if (error == GL_INVALID_VALUE)
			{
				Cl_Error("%s GL_INVALID_VALUE\n", name);
			}
			if (error == GL_INVALID_OPERATION)
			{
				Cl_Error("%s GL_INVALID_OPERATION\n", name);
			}
			if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
			{
				Cl_Error("%s GL_INVALID_FRAMEBUFFER_OPERATION\n", name);
			}
			if (error == GL_OUT_OF_MEMORY)
			{
				Cl_Error("%s GL_OUT_OF_MEMORY\n", name);
			}
		}

		static u32 OpenGLHelper::ShaderDataTypeOpenGL(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Float:  return GL_FLOAT;
			case ShaderDataType::Float2: return GL_FLOAT;
			case ShaderDataType::Float3: return GL_FLOAT;
			case ShaderDataType::Float4: return GL_FLOAT;
			case ShaderDataType::UInt:   return GL_UNSIGNED_INT;
			case ShaderDataType::Int:    return GL_INT;
			case ShaderDataType::Int2:   return GL_INT;
			case ShaderDataType::Int3:   return GL_INT;
			case ShaderDataType::Int4:   return GL_INT;
			case ShaderDataType::Mat3:   return GL_FLOAT;
			case ShaderDataType::Mat4:   return GL_FLOAT;
			case ShaderDataType::Bool:   return GL_BOOL;
			}

			return 0;
		}

		static GLenum OpenGLHelper::getGlenumFormat(const Texture::Format f)

		{

			switch (f)
			{
			case Texture::Format::RED:		return GL_RED;
			case Texture::Format::RGBA:		return GL_RGBA_INTEGER;
			case Texture::Format::RGB8:		return GL_RGB;
			case Texture::Format::RGBA8:	return GL_RGBA;
			case Texture::Format::RGBA16F:	return GL_RGBA;
			case Texture::Format::RGBA32F:	return GL_RGBA;
			case Texture::Format::RG32F:	return GL_RG;
			case Texture::Format::DEPTH32F:	return GL_DEPTH_COMPONENT32;
			case Texture::Format::DEPTH24STENCIL8:	return GL_DEPTH_STENCIL;
			}

			return GL_INVALID_ENUM;
		}

		static GLenum OpenGLHelper::getGlenumInternalFormat(const Texture::Format f)
		{
			switch (f)
			{
			case Texture::Format::RED: 		return GL_R8;
			case Texture::Format::RGBA: 	return GL_RGBA32UI;
			case Texture::Format::RGB8:		return GL_RGB8;
			case Texture::Format::RGBA8:	return GL_RGBA8;
			case Texture::Format::RGBA16F:	return GL_RGBA16F;
			case Texture::Format::RGBA32F:	return GL_RGBA32F;
			case Texture::Format::RG32F:	return GL_RG32F;
			case Texture::Format::DEPTH32F:	return GL_DEPTH_COMPONENT32;
			case Texture::Format::DEPTH24STENCIL8:	return GL_DEPTH24_STENCIL8;
			}

			return GL_INVALID_ENUM;
		}

		static GLenum OpenGLHelper::getGLWrap(const Texture::Wrap c)
		{

			switch (c)
			{
			case Texture::Wrap::REPEAT: return GL_REPEAT;
			case Texture::Wrap::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
			case Texture::Wrap::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
			}
			return GL_REPEAT;
		}

		static GLenum OpenGLHelper::getGLFilter(const Texture::Filter f)
		{
			switch (f)
			{
			case Texture::Filter::NEAREST: return GL_NEAREST;
			case Texture::Filter::LINEAR: return GL_LINEAR;
			case Texture::Filter::NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
			case Texture::Filter::LINEAR_MIPMAP_NEAREST: return GL_LINEAR_MIPMAP_NEAREST;
			case Texture::Filter::NEAREST_MIPMAP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
			case Texture::Filter::LINEAR_MIPMAP_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
			}

			return GL_LINEAR;
		}
	};
	

#endif