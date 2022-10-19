#include "Clover.h"

#include <Graphics/renderer.h>
#include <Graphics/renderer2D.h>
#include <Graphics/render_command.h>
#include <Graphics/material.h>

#include <Tools/CommandList.h>
 
	RendererApi::API RendererApi::Api = RendererApi::API::OpenGL;


	static Renderer* rendererInstance_ = new Renderer();

	struct RendererData
	{
		glm::mat4 viewProjection;
		Ref<RenderPass> m_ActiveRenderPass;
		DisplayList m_DisplayList;
		DisplayList m_RenderDisplayList;
		Ref<ShaderLibrary> m_ShaderLibrary;

		Ref<VertexBuffer> m_FullscreenQuadVertexBuffer;
		Ref<IndexBuffer> m_FullscreenQuadIndexBuffer;
		//Ref<Pipeline> m_FullscreenQuadPipeline;
	};

	static RendererData s_Data;
	static Renderer::Statistics s_Stats;


	void Renderer::ResetStats()
	{
		s_Stats.Meshes = 0;
		s_Stats.DrawCalls = 0;
	}


	Renderer::Statistics Renderer::GetStats()
	{
		return s_Stats;
	}

	void Renderer::Init()
	{
		RendererCommand::Init();

	}

	void Renderer::Swap()
	{
		s_Data.m_RenderDisplayList.Clear();
		s_Data.m_RenderDisplayList.Copy(s_Data.m_DisplayList);
		s_Data.m_DisplayList.Clear();
	}	

	void Renderer::BeginScene(const Camera& camera, const CMat4& transform)
	{
		s_Data.viewProjection = camera.GetProjection() * glm::inverse(transform);
	}


	void Renderer::BeginScene(const EditorCamera& camera)
	{
	//	s_Data.viewProjection = camera.GetViewProjection();
	}


	void Renderer::EndScene()
	{
		// Nothing
		s_Data.viewProjection = glm::mat4(1.0f);
	}


	void Renderer::RenderMesh(Ref<Mesh> mesh, CMat4& transform)
	{
		Ref<MaterialInstance> material;
		material = mesh->GetMaterial();

		//Prepare specific uniforms
		material->setUniform(ShaderDataType::Mat4, "u_ViewProjection", glm::value_ptr(s_Data.viewProjection));
		material->setUniform(ShaderDataType::Mat4, "u_Transform", glm::value_ptr(transform));
		int id = 1;
		material->setUniform(ShaderDataType::Int, "u_ObjectID", &id);
		material->setUniforms();
		Ref<Shader> shader = material->getShaderRef();
		Renderer::Submit([shader]() {
			shader->bind();
			});
		// Bind Textures and Upload Uniforms
		Renderer::Submit([material]() {
			material->uploadUniforms();
			});

		//Render
		Renderer::Submit([mesh]() {
			if (mesh)
			{
				RendererCommand::DrawIndexed(mesh->Raw()->vertexArray_);
			}
			});

		s_Stats.Meshes++;
		s_Stats.DrawCalls++;
	}

	void Renderer::WaitAndRender()
	{
		s_Data.m_RenderDisplayList.Execute();
	}	

	Renderer* Renderer::get()
	{
		return rendererInstance_;
	}

	Renderer::~Renderer()
	{}


	DisplayList& Renderer::GetRenderCommandQueue()
	{
		return s_Data.m_DisplayList;
	}
