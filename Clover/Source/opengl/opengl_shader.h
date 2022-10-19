#ifndef OPENGL_SHADER_H_
#define OPENGL_SHADER_H_ 1

#include "opengl.h"
#include "Graphics/shader.h"


    class OpenGLShader : public Shader {
    public:

      void bind() const override;
      void unBind() const override;
      void recompile() override;

      s32 get_attrib_location(const CString& name) const override; // Check use

      s32  get_uniform_position(const CString& name) const override;

      // Set the uniforms
      void setFloat(const CString& name, const float data) override;
      void setFloat2(const CString& name, const CVec2& data) override;
      void setFloat3(const CString& name, const CVec3& data) override;
      void setFloat4(const CString& name, const CVec4& data) override;

      void setIntArray(const CString& name, int* values, uint32_t count) override;
      void setInt(const CString& name, const int data) override;
	  void setInt2(const CString& name, const CVec2& data) override;
	  void setInt3(const CString& name, const CVec3& data) override;
	  void setInt4(const CString& name, const CVec4& data) override;

      void setMat3(const CString& name, const CMat3& data) override;
      void setMat4(const CString& name, const CMat4& data) override;
     
      void uploadUniforms(UniformBuffer& uniforms) override;


	OpenGLShader(const CString& name, const CString& filepath);
    OpenGLShader(const CString& name, const CString& fragment, const CString& vertex);
    ~OpenGLShader();



    void setFloat(const CString& name, const float* data) override;
    void setFloat2(const CString& name, const float* data) override;
    void setFloat3(const CString& name, const float* data) override;
    void setFloat4(const CString& name, const float* data) override;

    void setInt(const CString& name, const int* data) override;
    void setInt2(const CString& name, const int* data) override;
    void setInt3(const CString& name, const int* data) override;
    void setInt4(const CString& name, const int* data) override;

    void setMat3(const CString& name, const float* data) override;
    void setMat4(const CString& name, const float* data) override;



    private:
        //Read the file
		CString ReadFile(const CString& filepath);
        //Divide the source code into 2 shaders
		CMap<GLenum, CString> PreProcess(const CString& source);
        //Compile all shaders
		void Compile(const CMap<GLenum, CString>& shaderSources);


		u32 render_id;

  };

#endif
