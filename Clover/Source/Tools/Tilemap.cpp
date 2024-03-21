#include "Tilemap.h"
#include "stb_image.h"
#include "stb_image_resize.h"

Tilemap::Tilemap(const uint16 x, const uint16 y)
{
	Tiles = new Tile[x * y];
	Width = x;
	Height = y;
}

Tilemap::~Tilemap()
{
	delete[] Tiles;
}

Tile* Tilemap::Get(const uint16 x, const uint16 y)
{
	return &Tiles[x + y * Width];
}

void Tilemap::SetTile(const uint16 x, const uint16 y, const Tile& info)
{
	Tiles[x + y * Width] = info;
}

void Tilemap::Build(const uint16 Tile_Width, const uint16 Tile_Height, Image TextureMap[], const uint16 maxType, Ref<Texture2D>& OutTexture, bool resize)
{
	Image* ResizedTextureMap = nullptr;
	if (resize)
	{
		ResizedTextureMap = new Image[maxType + 1];
		for (size_t i = 0; i <= maxType; i++)
		{
			ResizedTextureMap[i].Create(TextureMap[i].width, TextureMap[i].height);
			ResizedTextureMap[i].DrawImage(&TextureMap[i], 0, 0);
			ResizedTextureMap[i].Resize(Tile_Width, Tile_Height);
		}
	}


	Texture::TextureProps props;
	props.format = Texture::Format::RGBA8;
	props.filter = Texture::Filter::NEAREST;
	OutTexture = GraphicContext::CreateTexture2D(Width * Tile_Width, Height * Tile_Height, props);

	Image image(Width * Tile_Width, Height * Tile_Height);
	for (int y = 0; y < Height; y++)
		for (int x = 0; x < Width; x++)
		{
			Tile* tile = Get(x, y);
			if (resize)
			{
				image.DrawImage(&ResizedTextureMap[tile->Type], x * Tile_Width, y * Tile_Height);
			}
			else
			{
				image.DrawImage(&TextureMap[tile->Type], x * Tile_Width, y * Tile_Height);
			}
		}


	OutTexture->setData(image.pixels, 0);

	delete[] ResizedTextureMap;
}
