#include "Clover.h"

#include <Graphics/renderer.h>
#include <Graphics/mesh.h>

#include <imgui.h>


	RawMesh::RawMesh(const std::string& name)
	{
		vertexArray_ = GraphicContext::CreateVertexArray();
		name_ = name;
	}


	RawMesh::RawMesh(const std::string& name, Ref<VertexArray>& vertexArray)
	{
		vertexArray_ = vertexArray;
		name_ = name;
	}

	Ref<RawMesh> RawMesh::CreateCube()
	{
		Ref<VertexBuffer> vertexBuffer_;
		Ref<IndexBuffer> indexBuffer_;


		float cubeVertices[12 * 24] = {
			 -0.5f,  -0.5f,  -0.5f,		0.375f, 1.00f,	 -1.0f,   0.0f,   0.0f,
			 -0.5f,  -0.5f,   0.5f,		0.625f, 1.00f,	 -1.0f,   0.0f,   0.0f,
			 -0.5f,   0.5f,   0.5f,		0.625f, 0.75f,	 -1.0f,   0.0f,   0.0f,
			 -0.5f,   0.5f,  -0.5f,		0.375f, 0.75f,	 -1.0f,   0.0f,   0.0f,
			 -0.5f,   0.5f,  -0.5f,		0.375f, 0.75f,	  0.0f,   1.0f,   0.0f,
			 -0.5f,   0.5f,   0.5f,		0.625f, 0.75f,	  0.0f,   1.0f,   0.0f,
			  0.5f,   0.5f,   0.5f,		0.625f, 0.50f,	  0.0f,   1.0f,   0.0f,
			  0.5f,   0.5f,  -0.5f,		0.375f, 0.50f,	  0.0f,   1.0f,   0.0f,
			  0.5f,   0.5f,  -0.5f,		0.375f, 0.50f,	  1.0f,   0.0f,   0.0f,
			  0.5f,   0.5f,   0.5f,		0.625f, 0.50f,	  1.0f,   0.0f,   0.0f,
			  0.5f,  -0.5f,   0.5f,		0.625f, 0.25f,	  1.0f,   0.0f,   0.0f,
			  0.5f,  -0.5f,  -0.5f,		0.375f, 0.25f,	  1.0f,   0.0f,   0.0f,
			  0.5f,  -0.5f,  -0.5f,		0.375f, 0.25f,	  0.0f,  -1.0f,   0.0f,
			  0.5f,  -0.5f,   0.5f,		0.625f, 0.25f,	  0.0f,  -1.0f,   0.0f,
			 -0.5f,  -0.5f,   0.5f,		0.625f, 1.00f,	  0.0f,  -1.0f,   0.0f,
			 -0.5f,  -0.5f,  -0.5f,		0.375f, 1.00f,	  0.0f,  -1.0f,   0.0f,
			 -0.5f,   0.5f,  -0.5f,		0.125f, 0.50f,	  0.0f,   0.0f,  -1.0f,
			  0.5f,   0.5f,  -0.5f,		0.375f, 0.50f,	  0.0f,   0.0f,  -1.0f,
			  0.5f,  -0.5f,  -0.5f,		0.375f, 0.25f,	  0.0f,   0.0f,  -1.0f,
			 -0.5f,  -0.5f,  -0.5f,		0.125f, 0.25f,	  0.0f,   0.0f,  -1.0f,
			  0.5f,   0.5f,   0.5f,		0.625f, 0.50f,	  0.0f,   0.0f,   1.0f,
			 -0.5f,   0.5f,   0.5f,		0.875f, 0.50f,	  0.0f,   0.0f,   1.0f,
			 -0.5f,  -0.5f,   0.5f,		0.875f, 0.25f,	  0.0f,   0.0f,   1.0f,
			  0.5f,  -0.5f,   0.5f,		0.625f, 0.25f,	  0.0f,   0.0f,   1.0f
		};

		vertexBuffer_ = GraphicContext::CreateVertexBuffer(cubeVertices, 12 * 24);
		VertexBufferLayout layout =
		{
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float3, "a_Normal"}
		};

		vertexBuffer_->setLayout(layout);

		unsigned int cubeIndices[36] = { 0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10,
			8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 };

		indexBuffer_ = GraphicContext::CreateIndexBuffer(cubeIndices, 36);
		
		
		Ref<VertexArray> vertexArray = GraphicContext::CreateVertexArray();
		vertexArray->addVertexBuffer(vertexBuffer_);
		vertexArray->setIndexBuffer(indexBuffer_);
		return CreateRef<RawMesh>("Cube", vertexArray);
	}

	Ref<RawMesh> RawMesh::CreateQuad()
	{
		Ref<VertexBuffer> vertexBuffer_;
		Ref<IndexBuffer> indexBuffer_;

		float squareVertices[4 * 8] = {
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		1.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		1.0f, 1.0f, 1.0f,
			 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,		1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,		1.0f, 1.0f, 1.0f
		};
		vertexBuffer_ = GraphicContext::CreateVertexBuffer(squareVertices, 4 * 8);
		VertexBufferLayout layout =
		{
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float3, "a_Normal"}
		};

		vertexBuffer_->setLayout(layout);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		indexBuffer_ = GraphicContext::CreateIndexBuffer(squareIndices, 6);


		Ref<VertexArray> vertexArray = GraphicContext::CreateVertexArray();
		vertexArray->addVertexBuffer(vertexBuffer_);
		vertexArray->setIndexBuffer(indexBuffer_);
		return CreateRef<RawMesh>("Quad", vertexArray);
	}

	// Improve sphere creation
	Ref<RawMesh> RawMesh::CreateSphere()
	{
		CVector<CVec3> positions;
		CVector<CVec2> uv;
		CVector<CVec3> normals;
		CVector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = (float)std::cos(xSegment * 2.0f * PI) * (float)std::sin(ySegment * PI);
				float yPos = (float)std::cos(ySegment * PI);
				float zPos = (float)std::sin(xSegment * 2.0f * PI) * (float)std::sin(ySegment * PI);

				positions.push_back(CVec3(xPos, yPos, zPos));
				uv.push_back(CVec2(xSegment, ySegment));
				normals.push_back(CVec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		s32 indexCount = (s32)indices.size();

		CVector<float> data;
		for (size_t i = 0; i < positions.size(); ++i)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);

			data.push_back(uv[i].x);
			data.push_back(uv[i].y);

			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);

		}

		Ref<VertexBuffer> vertexBuffer_;
		Ref<IndexBuffer> indexBuffer_;

		vertexBuffer_ = GraphicContext::CreateVertexBuffer(data.data(), (u32)data.size());
		VertexBufferLayout layout =
		{
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float3, "a_Normal"}
		};

		vertexBuffer_->setLayout(layout);

		indexBuffer_ = GraphicContext::CreateIndexBuffer(indices.data(), indexCount);


		Ref<VertexArray> vertexArray = GraphicContext::CreateVertexArray();
		vertexArray->addVertexBuffer(vertexBuffer_);
		vertexArray->setIndexBuffer(indexBuffer_);
		return CreateRef<RawMesh>("Sphere", vertexArray);
	}



	Mesh::Mesh(const Ref<RawMesh>& rawMesh)
		: raw_(rawMesh)
	{
		//SetMaterial(Asset::Materials().Get(DEFAULT_UNLIT));

		name_ = raw_->name_;
	}


	Mesh::Mesh(const std::string& filepath)
	{

	}
