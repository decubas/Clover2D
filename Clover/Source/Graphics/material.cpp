#include "Clover.h"

#include <Graphics/graphiccontext.h>
#include <Graphics/material.h>

 
	Material::Material(const Ref<Shader>& shader)
	{
		setShader(shader);
	}

	void Material::setShader(const Ref<Shader>& shader)
	{
		shader_ = shader;
	}

	Ref<Shader>& Material::getShader()
	{
		return shader_;
	}


	void Material::uploadUniforms(UniformBuffer& uniforms)
	{
		shader_->uploadUniforms(uniforms);
	}

//		shader_->setFloat4("u_Color", materialIns.uniform_.baseColor_);
// 		shader_->setFloat("u_Metallic", materialIns.uniform_.metallic_);
// 		shader_->setFloat("u_Shininess", materialIns.uniform_.metallic_);
// 		shader_->setFloat("u_AmbientOcclusion", materialIns.uniform_.ambientOcclusion_);
// 		shader_->setFloat("u_Roughness", materialIns.uniform_.roughness_);
// 		shader_->setFloat("u_Tiling", materialIns.uniform_.tiling_);
// 
// 		shader_->setFloat("u_Ambient", materialIns.uniform_.ambient_);
// 		shader_->setFloat("u_Diffuse", materialIns.uniform_.diffuse_);
// 		shader_->setFloat("u_Specular", materialIns.uniform_.specular_);
// 
// 		//Lighting
// 		shader_->setFloat3("u_LightUse", materialIns.uniform_.lightUse_);
// 		shader_->setFloat3("u_EyePos", materialIns.uniform_.eyePos_);

// 		{
// 			MTR_SCOPE("Material", "Light Use");
// 			int i = 0;
// 			for (const LightData& data_ : materialIns.uniform_.lightData_)
// 			{
// 				CString i_str = std::to_string(i);
// 				shader_->setMat4(("u_lights_vp_matrix[" + i_str + "]"), data_.shadow_.viewProjectionMatrix_);
// 
// 				CString string_ = "lights[" + i_str + "].";
// 				shader_->setFloat(string_ + "intensity", data_.intensity);
// 
// 				shader_->setFloat(string_ + "outCutOff", data_.outCutOff);
// 				shader_->setFloat(string_ + "cutOff", data_.cutOff);
// 				shader_->setFloat(string_ + "linear", data_.linear);
// 				shader_->setFloat(string_ + "quadratic", data_.quadratic);
// 				shader_->setFloat(string_ + "constant", data_.constant);
// 				shader_->setFloat3(string_ + "direction", data_.direction);
// 				shader_->setFloat3(string_ + "position", data_.position);
// 				shader_->setFloat4(string_ + "color", data_.color);
// 				shader_->setMat4(string_ + "vp_matrix", data_.shadow_.viewProjectionMatrix_);
// 				i++;
// 			}
// 
// 		}


// 		shader_->setInt("u_depth", DEPTH_TEXTURE_ID);
// 		shader_->setInt("u_depth_1", DEPTH_TEXTURE_ID + 1);
// 		shader_->setInt("u_depth_2", DEPTH_TEXTURE_ID + 2);
// 		shader_->setInt("u_depth_3", DEPTH_TEXTURE_ID + 3);


/////////////////////////////

	MaterialInstance::MaterialInstance(const Ref<Material>& material)
		: material_(material)
	{
		u32 whiteTex = 0xffffffff;
		u32 blackTex = 0x00000000;

		Texture::TextureProps props;
		props.format = Texture::Format::RGBA16F;
		Ref<Texture2D> defaultDiffTexture_ = GraphicContext::CreateTexture2D(1, 1, props);
		defaultDiffTexture_->setData(&whiteTex, sizeof(u32));

		props.format = Texture::Format::RGBA16F;
		Ref<Texture2D> defaultSpecTexture_ = GraphicContext::CreateTexture2D(1, 1, props);
		defaultSpecTexture_->setData(&blackTex, sizeof(u32));

		Ref<Texture2D> defaultRoughtTexture_ = GraphicContext::CreateTexture2D(1, 1, props);
		defaultRoughtTexture_->setData(&blackTex, sizeof(u32));

		Ref<Texture2D> defaultNormalTexture_ = GraphicContext::CreateTexture2D(1, 1, props);
		defaultNormalTexture_->setData(&whiteTex, sizeof(u32));

		Ref<Texture2D> defaultAOTexture_ = GraphicContext::CreateTexture2D(1, 1, props);
		defaultAOTexture_->setData(&whiteTex, sizeof(u32));

		textures_[DIFFUSE_TEXTURE] = ShaderTexture(defaultDiffTexture_, DEFAULT_TEXTURES);
		textures_[SPECULAR_TEXTURE] = ShaderTexture(defaultSpecTexture_, DEFAULT_TEXTURES + 1);
// 
 		//PBR
		textures_[ROUGH_TEXTURE] = ShaderTexture(defaultRoughtTexture_, DEFAULT_TEXTURES + 2);
		textures_[NORMAL_TEXTURE] = ShaderTexture(defaultNormalTexture_, DEFAULT_TEXTURES + 3);
		textures_[AMBIENT_OCLUSION_TEXTURE] = ShaderTexture(defaultAOTexture_, DEFAULT_TEXTURES + 4);

		
// 
// 		uniform_.ambient_ = 1;
// 		uniform_.diffuse_ = 1;
// 		uniform_.specular_ = 1;

		resetTextures();
	}


	void MaterialInstance::setUniforms()
	{
		//Specific Uniforms
		setUniform(ShaderDataType::Float4, "u_Color", glm::value_ptr(Uniforms.baseColor_));

		/////

		PrepareTexturesUniforms();
		SwapUniforms();
	}

	void MaterialInstance::uploadUniforms()
	{
		bindTextures();
		material_->uploadUniforms(uniforms_buffer_);
		swap = true;
	}

	void MaterialInstance::bindTextures()
	{
		for (auto& t : textures_)
		{
			t.second.tex_->bind(t.second.id_);
		}
	}

	void MaterialInstance::replaceTexture(const CString& name, const Ref<Texture2D>& texture)
	{
		if (textures_.find(name) == textures_.end())
		{
			Cl_Info("This shader does not have {0} defined!!", name.c_str());
			return;
		}

		textures_[name].tex_ = texture;
	}

	void MaterialInstance::setTexture(const CString& name, const Ref<Texture2D>& texture)
	{
		replaceTexture(name, texture);
	}

	void MaterialInstance::setAllTextures(const Ref<Texture2D>& texture)
	{
		for (auto& t : textures_)
		{
			replaceTexture(t.first, texture);
		}
	}

	void MaterialInstance::resetTextures()
	{
		Uniforms.alpha_ = 1.0f;
		Uniforms.tiling_ = 1.0f;
		Uniforms.metallic_ = 1.0f;
		Uniforms.roughness_ = 1.0f;
		Uniforms.ambientOcclusion_ = 1.0f;
		Uniforms.baseColor_ = glm::vec4(1.0f);

		maxTextures = DEFAULT_TEXTURES + 4;

	}

	Ref<Texture2D>& MaterialInstance::getTexture(const CString& name)
	{
		return textures_[name].tex_;
	}

	void MaterialInstance::ImGuiEditor()
	{
// 		ImGui::Text("Material Settings");
// 		{
// 			ImGui::ColorEdit4("BaseColor", glm::value_ptr(Uniforms.baseColor_));
// 			ImGui::Separator();
// 			for (auto& t : textures_)
// 			{
// 				ImGui::Text(t.first.c_str());
// 				t.second.tex_->ImGuiEditor();
// 				ImGui::Separator();
// 			}
// 			ImGui::Separator();


//			ImGui::DragFloat("Ambient", &(uniform_.ambient_), 0.001f, 0, 1);
//			ImGui::DragFloat("Diffuse", &(uniform_.diffuse_), 0.001f, 0, 1);
//			ImGui::DragFloat("Specular", &(uniform_.specular_), 0.001f, 0, 1);

			//ImGui::Text("PBR Settings");
// 			ImGui::DragFloat("Metallic", &uniform_.metallic_, 0.001f, 0, 1);
// 			ImGui::DragFloat("Roughness", &uniform_.roughness_, 0.001f, 0, 1);
// 			ImGui::DragFloat("Ambient Occlusion", &uniform_.ambientOcclusion_, 0.001f, 0, 1);
// 			ImGui::Separator();
// 			ImGui::DragFloat("Alpha", &uniform_.alpha_, 0.001f, -1, 1);
// 			ImGui::DragFloat("Texture Tiling", &uniform_.tiling_, 0.001f, 0.f);

//		}
	}



	void MaterialInstance::PrepareTexturesUniforms()
	{
		for (auto& t : textures_)
		{
			setUniform(ShaderDataType::Int, t.first, &t.second.id_);
		}
	}

	void MaterialInstance::SwapUniforms()
	{
		if (swap)
		{
			uniforms_buffer_.clear();
			uniforms_buffer_.swap(uniforms_);
			uniforms_.clear();
			swap = false;
		}
	}
