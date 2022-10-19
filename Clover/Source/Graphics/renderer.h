#ifndef _BASE_RENDER2D_H
#define _BASE_RENDER2D_H

#include <Clover.h>

#include <Utils/Library.h>
#include <Tools/CommandList.h>

#include "graphiccontext.h"
#include "camera.h"
#include "mesh.h"


	enum class DepthFunc {
		k_Never = 0,
		k_Less,
		k_Greater,
		k_Equal,
		k_Always,
		k_LEqual,
		k_GEqual,
		k_NotEqual
	};

	enum class BlendFunc {
		k_Add = 0,
		k_Substract,
		k_ReverseSubstract,
		k_Min,
		k_Max
	};

	enum class BlendParameter {
		k_Zero = 0,
		k_One,

		k_SrcColor,
		k_DtsColor,
		k_ConsColor,
		k_OneMinusSrcColor,
		k_OneMinusDtsColor,
		k_OneMinusConsColor,

		k_SrcAlpha,
		k_DtsAlpha,
		k_ConsAlpha,
		k_OneMinusSrcAlpha,
		k_OneMinusDtsAlpha,
		k_OneMinusConsAlpha,

		k_SrcAlphaSaturate
	};

	enum class CullFace
	{
		k_Front,
		k_Back,
		k_Front_Back
	};

	// Interface of the Graphic API
	class RendererApi
	{
	public:
		enum class API
		{
			none = 0,
			OpenGL
		};


		virtual void Init() = 0;
		virtual void setClearColor(const glm::vec4& color = glm::vec4()) = 0;
		virtual void Clear() = 0;
		virtual void setCullFace(int cullFace) = 0;
		virtual void enable(int setting) = 0;
		virtual void disable(int setting) = 0;
		virtual void setDrawMode(int mode) = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) = 0;

		RendererApi() { }
		virtual ~RendererApi() {}

		inline static API GetApi() { return Api; }


	private:
		static API Api;
	};


	// Singleton, Main class in charge of rendering
    class Renderer
    {
		typedef void(*RenderCommandFn)(void*);
    public:

		// Stats
		struct Statistics
		{
			u32 DrawCalls = 0;
			u32 Meshes = 0;
		};
		static void ResetStats();
		static Statistics GetStats();

		static void Init();

		//Render list Swap
		static void WaitAndRender();
		static void Swap();

		static void BeginScene(const Camera& camera, const CMat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void EndScene();

		//Submit Commands
        static void RenderMesh(Ref<Mesh> mesh, CMat4& transform);

		template<typename FuncT>
		static void Submit(FuncT&& func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();

				// NOTE: Instead of destroying we could try and enforce all items to be trivally destructible
				// however some items like uniforms which contain std::strings still exist for now
				//static_assert(std::is_trivially_destructible_v<FuncT>, "FuncT must be trivially destructible");
				//pFunc->~FuncT();
			};
			auto storageBuffer = GetRenderCommandQueue().Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}
		

		static Renderer* get();

        inline static RendererApi::API getApi() { return RendererApi::GetApi(); }

		virtual ~Renderer();

    private:
		static DisplayList& GetRenderCommandQueue();
    };

#endif
