#ifndef BASE_SHADER_H_
#define BASE_SHADER_H_ 1

#include <Clover.h>

#include <Utils/Library.h>
#include <Utils/handler.h>


	typedef CMap<CString, CPair<ShaderDataType, Handler>> UniformBuffer;

  class Shader : public Cl_Object
  {
  public:

	virtual void bind() const = 0;
	virtual void unBind() const = 0;
    virtual void recompile() = 0;

    virtual s32 get_attrib_location(const CString& name) const = 0; // Check use

    virtual s32  get_uniform_position(const CString& name) const = 0;

	virtual void uploadUniforms(UniformBuffer& uniforms) = 0;
    
	virtual void setFloat(const CString& name, const float data) = 0;
	virtual void setFloat2(const CString& name, const CVec2& data) = 0;
	virtual void setFloat3(const CString& name, const CVec3& data) = 0;
	virtual void setFloat4(const CString& name, const CVec4& data) = 0;

	virtual void setIntArray(const CString& name, int* values, uint32_t count) = 0;
	virtual void setInt(const CString& name, const int data) = 0;
	virtual void setInt2(const CString& name, const CVec2& data) = 0;
	virtual void setInt3(const CString& name, const CVec3& data) = 0;
	virtual void setInt4(const CString& name, const CVec4& data) = 0;
	
	virtual void setMat3(const CString& name, const CMat3& data) = 0;
	virtual void setMat4(const CString& name, const CMat4& data) = 0;



	virtual void setFloat(const CString& name, const float* data) = 0;
	virtual void setFloat2(const CString& name, const float* data) = 0;
	virtual void setFloat3(const CString& name, const float* data) = 0;
	virtual void setFloat4(const CString& name, const float* data) = 0;

	virtual void setInt(const CString& name, const int* data) = 0;
	virtual void setInt2(const CString& name, const int* data) = 0;
	virtual void setInt3(const CString& name, const int* data) = 0;
	virtual void setInt4(const CString& name, const int* data) = 0;

	virtual void setMat3(const CString& name, const float* data) = 0;
	virtual void setMat4(const CString& name, const float* data) = 0;

	virtual const CString& getName() const { return name_; }



    Shader() {};
    ~Shader() {};

  protected:
      CString name_;
      CString path_;
  };
  


  class ShaderLibrary : public Library<Shader>
  {
  public:

      void recompile();
	  Ref<Shader> Load(const CString& name, const CString& filepath);

  };
 
#endif
