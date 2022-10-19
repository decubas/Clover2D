#ifndef _BASE_TEXTURE_
#define _BASE_TEXTURE_ 1

#include <Clover.h>

#include <Utils/Library.h>


	// Base Interface
    class Texture : public Cl_Object
	{
    public:


		enum class Format {
			F_None = 0,

			// Color
			RED = 8,
			RGBA = 7,
			RGB8 = 9,
			RGBA8 = 1,
			RGBA16F = 2,
			RGBA32F = 3,
			RG32F = 4,

			// Depth / Stencil
			DEPTH32F = 5,
			DEPTH24STENCIL8 = 6,

			//Defaults
			Depth = DEPTH24STENCIL8
		};

		enum class Filter {
			NEAREST,
			LINEAR,
			NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR
		};

		enum class Wrap {
			REPEAT,
			MIRRORED_REPEAT,
			CLAMP_TO_EDGE
		};

		struct TextureProps
		{
			Filter filter = Filter::LINEAR;
			Wrap wrap = Wrap::REPEAT;
			Format format = Format::RGBA8;
		};


        virtual void setData(const void* data, u32 size) = 0;
		virtual void resetTexture(const CString& filepath, bool relative = true) = 0;


		virtual void setMinFilter(Filter f) = 0;
		virtual void setMagFilter(Filter f) = 0;
		virtual void setWrapS(Wrap c) = 0;
		virtual void setWrapT(Wrap c) = 0;
		virtual void setWrapR(Wrap c) = 0;
		virtual void setWrap(Wrap c) = 0;

        virtual void generateMipmaps() const = 0;

		virtual void ImGuiEditor() = 0;
        
		inline u32 GetWidth() { return width_; }
		inline u32 GetHeight() { return height_; }
        virtual void bind(u32 slot = 0) const = 0;
        virtual void unBind(u32 slot = 0) const = 0;
		inline u32 id() const { return render_id; }
		bool operator==(const Texture& other) const
		{
			return render_id == other.render_id;
		}

		virtual void Delete() = 0;
        virtual ~Texture() = default;
        CString path_;
		CString shaderName_;

    protected:
        TextureProps props_;
		u32 width_;
		u32 height_;
        u32 depth_;
        u32 render_id;

    };

    class Texture2D : public Texture 
    {
    public:
		virtual ~Texture2D() = default;
    };

	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D> Tex, const glm::vec2& Coords, const glm::vec2& CellSize, const glm::vec2& SpriteSize = {1, 1});
		virtual ~SubTexture2D() = default;

		Ref<Texture2D> GetTexture() { return m_Texture; }
		glm::vec2* GetTexCoords() { return m_TexCoords; }


	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
	};

	class Texture2DLibrary : public Library<Texture2D>
	{
		Ref<Texture2D> defaultWhite;
		Ref<Texture2D> defaultBlack;
		Ref<Texture2D> defaultError;
	public:

		Texture2DLibrary();

		Ref<Texture2D> Load(const CString& name, const CString& filepath);

		Ref<Texture2D> DefaultWhite();
		Ref<Texture2D> DefaultBlack();
		Ref<Texture2D> DefaultError();
	
	};

	template<class TextureType>
	class Animation2D
	{
	public:
		Animation2D() = default;
		Animation2D(const std::vector<Ref<TextureType>>& Textures, float PlayRate = 1.0f);

		Ref<TextureType> GetCurrentTexture();

	protected:
		float m_AccumRate = 0.f;
		float m_PlayRate = 1.f;
		u32 m_Index = 0;
		std::vector<Ref<TextureType>> m_Textures;
	};

	template<class TextureType>
	Animation2D<TextureType>::Animation2D(const std::vector<Ref<TextureType>>& Textures, float PlayRate /*= 1.0f*/)
		: m_Textures(Textures), m_PlayRate(PlayRate), m_Index(0)
	{}

	template<class TextureType>
	Ref<TextureType> Animation2D<TextureType>::GetCurrentTexture()
	{
		u32 LastIndex = m_Index;

		m_AccumRate += m_PlayRate;
		if (m_AccumRate >= 1.0f)
		{
			m_AccumRate = 0.f;
			++m_Index;

			if (m_Index >= m_Textures.size())
				m_Index = 0;
		}

		return m_Textures[LastIndex];
	}

#endif
