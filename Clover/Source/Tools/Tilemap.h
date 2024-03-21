#pragma once

#include <CloverEngine.h>
#include "Image.h"

class Texture2D;

struct Tile
{
	uint8 Type = 0;
	void* Data = nullptr;
};

class Tilemap
{
public:
	Tilemap(const uint16 x, const uint16 y);
	~Tilemap();

	Tile* Get(const uint16 x, const uint16 y);

	void SetTile(const uint16 x, const uint16 y, const Tile& info);

	void Build(const uint16 Tile_Width, const uint16 Tile_Height, Image TextureMap[], const uint16 maxType, Ref<Texture2D>& OutTexture, bool resize = false);

	uint16 Width;
	uint16 Height;
private:
	Tile* Tiles;
};