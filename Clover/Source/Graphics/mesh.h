#ifndef _MESH_
#define _MESH_ 1

#include <Clover.h>

#include "pipeline.h"
#include "vertexarray.h"
#include "material.h"


	struct SubMesh
	{
		u32 baseVertex;
		u32 baseIndex;
		u32 materialIndex;
		u32 indexCount;

		CMat4 transform;

		CString nodeName, meshName;
	};

	//Resource
	class RawMesh : public Cl_Object
	{
	public:

		RawMesh(const CString& name, Ref<VertexArray>& vertexArray);
		RawMesh(const CString& name);
		virtual ~RawMesh() = default;

		static Ref<RawMesh> CreateCube();
		static Ref<RawMesh> CreateQuad();
		static Ref<RawMesh> CreateSphere();


		CString name_;
		Ref<VertexArray> vertexArray_;
		CVector<SubMesh> subMeshes_;
	};



	class Mesh : public Cl_Object
	{
	public:
		Mesh(const std::string& filepath);
		Mesh(const Ref<RawMesh>& rawMesh);
		~Mesh() = default;

		void Mesh::SetMaterial(const Ref<Material>& material)
		{
			baseMaterial_ = material;
			material_ = CreateRef<MaterialInstance>(baseMaterial_);
			pipeline_ = CreateRef<Pipeline>(baseMaterial_->getShader());
		}

		void Mesh::SetRawMesh(const Ref<RawMesh>& raw)
		{
			raw_ = raw;
		}

		CString& Name() { return name_; }
		Ref<RawMesh>& Raw() { return raw_; }
		Ref<Material>& BaseMaterial() { return baseMaterial_; }
		Ref<MaterialInstance>& GetMaterial() { return material_; }

	protected:
		CString name_;

		//Resource
		Ref<RawMesh> raw_;

		Ref<Pipeline> pipeline_;

		Ref<Material> baseMaterial_;
		Ref<MaterialInstance> material_;
	};

#endif
