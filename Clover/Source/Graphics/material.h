#ifndef _MATERIAL_
#define _MATERIAL_ 1

#include <Clover.h>

#include <Utils/handler.h>
#include "texture.h"
#include "shader.h"


	class MaterialInstance;


	class Material : public Cl_Object
	{
	public:
		friend class MaterialInstance;


		void setShader(const Ref<Shader>& shader);
		Ref<Shader>& getShader();

		void uploadUniforms(UniformBuffer& uniforms);

		Material(const Ref<Shader>& shader);
		virtual ~Material() = default;


		Ref<Shader> shader_;
	protected:

	};


	class MaterialInstance : public Cl_Object
	{
		friend Material;
		struct ShaderTexture
		{
			ShaderTexture() {}
			ShaderTexture(Ref<Texture2D> texture, u32 id) : tex_(texture), id_(id) {}

			Ref<Texture2D> tex_;
			u32 id_;
			bool use_ = false;

			ShaderTexture& operator=(ShaderTexture& other)
			{
				tex_ = other.tex_;
				id_ = other.id_;
				use_ = other.use_;
				return *this;
			}
		};
	public:
		

		struct Uniforms
		{
			CVec3 lightUse_;
			CVec3 eyePos_;
			//CVector<LightData> lightData_;
			float tiling_;
			float alpha_;

			float ambient_;
			float diffuse_;
			float specular_;

			//PBR
			float metallic_;
			float roughness_;
			float ambientOcclusion_;
			glm::vec4 baseColor_;

		} Uniforms;


		MaterialInstance(const Ref<Material>& material);

		void setUniforms();
		void uploadUniforms();
		void bindTextures();

		void replaceTexture(const CString& name, const Ref<Texture2D>& texture);
		void setTexture(const CString& name, const Ref<Texture2D>& texture);
		void setAllTextures(const Ref<Texture2D>& texture);
		void resetTextures();
		Ref<Texture2D>& getTexture(const CString& name);
		s32 getCurrentTextures() { return currentTextures; }
		Shader& getShader() { return *material_->getShader(); }
		Ref<Shader>& getShaderRef() { return material_->getShader(); }

		Ref<Material>& GetMaterial() { return material_; };

		void ImGuiEditor();

		void setUniform(const ShaderDataType type, const std::string& name, void* value)
		{
			bool found = uniforms_.find(name) != uniforms_.end();
			if (!found)
			{
				uniforms_[name] = { type, Handler(value, SizeOfShaderDataType(type)) };
			}
			uniforms_[name].second.Alloc(value, SizeOfShaderDataType(type));
		}

		CString name_;

		CMap<CString, ShaderTexture> textures_;

	protected:
		bool swap = false;
		UniformBuffer uniforms_;
		UniformBuffer uniforms_buffer_;

		void PrepareTexturesUniforms();
		void SwapUniforms();
		s32 currentTextures = 0;
		s32 maxTextures;
		Ref<Material> material_;
		
		Ref<Texture2D> defaultTexture_;
	};

#endif
