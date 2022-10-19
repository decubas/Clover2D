
#include <Clover.h>
#include <opengl/opengl_shader.h>
#include <Graphics/renderer.h>


	static GLenum ShaderTypeFromString(const CString& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		Cl_Assert(false, "Unknown shader type!");
		return 0;
	}



	OpenGLShader::OpenGLShader(const CString& name, const CString& filepath)
	{
		name_ = name;
		path_ = AssetsPath + filepath;
		CString source = ReadFile(path_);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);		
	}

	OpenGLShader::OpenGLShader(const CString& name, const CString& fragment, const CString& vertex)
	{
		name_ = name;

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertex;
		sources[GL_FRAGMENT_SHADER] = fragment;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(render_id);


	}


	CString OpenGLShader::ReadFile(const CString& filepath)
	{
		CString result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
				in.close();
			}
			else
			{
				Cl_Error("Could not read from file %s", filepath.c_str());
			}
		}
		else
		{
			Cl_Error("Could not open file %s", filepath.c_str());
		}

		return result;
	}

	CMap<GLenum, CString> OpenGLShader::PreProcess(const CString& source)
	{
		std::unordered_map<GLenum, CString> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != CString::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			Cl_Assert(eol != CString::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			CString type = source.substr(begin, eol - begin);
			Cl_Assert(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			Cl_Assert(nextLinePos != CString::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[ShaderTypeFromString(type)] = (pos == CString::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const CMap<GLenum, CString>& shaderSources)
	{
		GLuint program = glCreateProgram();

		std::array<GLenum, 2> glShaderIDs; // Currently supported 2 Shaders
		s32 glShaderIDIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const CString& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
				Cl_Error("SHADER::COMPILE::ERROR: %s", infoLog.data());

				glDeleteShader(shader);

				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		render_id = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (s32*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			Cl_Error("PROGRAM::LINK::ERROR: %s", infoLog.data());


			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			return;
		}

		//Get all uniforms
		/*
		GLint count;

		GLint size; // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 16; // maximum name length
		GLchar name[bufSize]; // variable name in GLSL
		GLsizei length; // name length

		glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
		printf("Active Attributes: %d\n", count);

		for (int i = 0; i < count; i++)
		{
			glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, name);

			printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
		}

		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
		printf("Active Uniforms: %d\n", count);
		for (int i = 0; i < count; i++)
		{
			glGetActiveUniform(program, (GLuint)i, 100, &length, &size, &type, name);

			printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
		}*/

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

	}



	void OpenGLShader::bind() const
    {
		glUseProgram(render_id);		
    }

	void OpenGLShader::unBind() const
	{
        glUseProgram(0);
	}

	void OpenGLShader::recompile()
	{
		CString source = ReadFile(path_);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);
	}

	int OpenGLShader::get_attrib_location(const CString& name) const {

        GLint location = glGetAttribLocation(render_id, name.c_str());;

        glBindAttribLocation(render_id, location, name.c_str());

        return location;
    }


    int OpenGLShader::get_uniform_position(const CString& name) const {

        return glGetUniformLocation(render_id, name.c_str());
    }

	void OpenGLShader::setIntArray(const CString& name, int* values, uint32_t count)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniform1iv((GLint)uniform_pos, count, (GLint*)values);
	}

	void OpenGLShader::setFloat(const CString& name, const float value)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniform1f(uniform_pos, value);
	}


	void OpenGLShader::setFloat(const CString& name, const float* data)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniform1fv(uniform_pos, 1, data);
	}

	void OpenGLShader::setFloat2(const CString& name, const CVec2& value)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniform2f(uniform_pos, value.x, value.y);
	}


	void OpenGLShader::setFloat2(const CString& name, const float* data)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniform2fv(uniform_pos, 1, data);
	}

	void OpenGLShader::setFloat3(const CString& name, const CVec3& value)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniform3f(uniform_pos, value.x, value.y, value.z);
	}


	void OpenGLShader::setFloat3(const CString& name, const float* data)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniform3fv(uniform_pos, 1, data);
	}

	void OpenGLShader::setFloat4(const CString& name, const CVec4& value)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniform4f(uniform_pos, value.x, value.y, value.z, value.w);
	}


	void OpenGLShader::setFloat4(const CString& name, const float* data)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniform4fv(uniform_pos, 1, data);
	}

	void OpenGLShader::setInt(const CString& name, int value)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniform1i(uniform_pos, (GLint)value);
	}


	void OpenGLShader::setInt(const CString& name, const int* data)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniform1iv(uniform_pos, 1, data);
	}

	void OpenGLShader::setInt2(const CString& name, const CVec2& value)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniform2i(uniform_pos, (GLint)value.x, (GLint)value.y);
	}


	void OpenGLShader::setInt2(const CString& name, const int* data)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniform2iv(uniform_pos, 1, data);
	}

	void OpenGLShader::setInt3(const CString& name, const CVec3& value)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniform3i(uniform_pos, (GLint)value.x, (GLint)value.y, (GLint)value.z);
	}


	void OpenGLShader::setInt3(const CString& name, const int* data)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniform3iv(uniform_pos, 1, data);
	}

	void OpenGLShader::setInt4(const CString& name, const CVec4& value)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniform4i(uniform_pos, (GLint)value.x, (GLint)value.y, (GLint)value.z, (GLint)value.w);
	}


	void OpenGLShader::setInt4(const CString& name, const int* data)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniform4iv(uniform_pos, 1, data);
	}

	void OpenGLShader::setMat3(const CString& name, const CMat3& matrix)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniformMatrix3fv(uniform_pos, 1, false, glm::value_ptr(matrix));
	}


	void OpenGLShader::setMat3(const CString& name, const float* data)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniformMatrix3fv(uniform_pos, 1, false, data);
	}

	void OpenGLShader::setMat4(const CString& name, const CMat4& matrix)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniformMatrix4fv(uniform_pos, 1, false, glm::value_ptr(matrix));
	}

	void OpenGLShader::setMat4(const CString& name, const float* data)
	{
		s32 uniform_pos = get_uniform_position(name);
		if (uniform_pos != -1)  glUniformMatrix4fv(uniform_pos, 1, false, data);
	}


	void OpenGLShader::uploadUniforms(UniformBuffer& uniforms)
	{
		if (!uniforms.empty())
		{
			for (auto& uniform : uniforms)
			{
				switch (uniform.second.first)
				{
				case ShaderDataType::Float:  setFloat(uniform.first, uniform.second.second.get<float>());	break;
				case ShaderDataType::Float2: setFloat2(uniform.first, uniform.second.second.get<float>());	break;
				case ShaderDataType::Float3: setFloat3(uniform.first, uniform.second.second.get<float>());	break;
				case ShaderDataType::Float4: setFloat4(uniform.first, uniform.second.second.get<float>());	break;
				case ShaderDataType::UInt:   setInt(uniform.first, (s32*)uniform.second.second.get<u32>()); break;
				case ShaderDataType::Int:    setInt(uniform.first, uniform.second.second.get<int>());		break;
				case ShaderDataType::Int2:   setInt2(uniform.first, uniform.second.second.get<int>());		break;
				case ShaderDataType::Int3:   setInt3(uniform.first, uniform.second.second.get<int>());		break;
				case ShaderDataType::Int4:   setInt4(uniform.first, uniform.second.second.get<int>());		break;
				case ShaderDataType::Mat3:   setMat3(uniform.first, uniform.second.second.get<float>());	break;
				case ShaderDataType::Mat4:	 setMat4(uniform.first, uniform.second.second.get<float>());	break;
				case ShaderDataType::Bool:   /*shader_->setBool();*/break;
				}
			}
		}

	}
	