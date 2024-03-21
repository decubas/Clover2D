#pragma once

#include <CloverEngine.h>


struct Pixel
{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 0xFF;
};


struct Image
{
	Pixel* pixels = nullptr;
	uint16 width = 0;  // in pixels
	uint16 height = 0;

	Image() = default;

	Image(const uint16 w, const uint16 h) : width(w), height(h)
	{
		pixels = new Pixel[w * h];
	}

	~Image()
	{
		delete[] pixels;
		pixels = nullptr;
	}

	Image(const char* file);

	void Create(const uint16 w, const uint16 h)
	{
		width = w;
		height = h;
		pixels = new Pixel[w * h];
	}
	void SetPixel(const uint16 x, const uint16 y, const Pixel& color)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return; // out of bounds

		pixels[width * y + x] = color;
	}

	Pixel GetPixel(const uint16 x, const uint16 y) const
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
			return Pixel(); // out of bounds, return a default value

		return pixels[width * y + x];
	}

	void Resize(const uint16 w, const uint16 h);

	void DrawImage(const Image* src, const uint16 to_x, const uint16 to_y)
	{
		for (int from_y = 0; from_y < src->height; from_y++)
		{
			for (int from_x = 0; from_x < src->width; from_x++)
			{
				int x = to_x + from_x;
				int y = to_y + from_y;
				Pixel color = src->GetPixel(from_x, from_y);

				SetPixel(x, y, color);
			}
		}
	}

	void DrawImageFromSource(const unsigned char* src, const uint16 w, const uint16 h, const uint16 to_x, const uint16 to_y)
	{

		for (int from_y = 0; from_y < h; from_y++)
		{
			for (int from_x = 0; from_x < w; from_x++)
			{
				const unsigned char* current = (const unsigned char*)&src[(from_x*4) + from_y * (w*4)];

				Pixel color;
				color.r = current[0];
				color.g = current[1];
				color.b = current[2];
				color.a = current[3];

				int x = to_x + from_x;
				int y = to_y + from_y;
				SetPixel(x, y, color);
			}
		}
		return;

		for (int i = 0; i < (w * h * 4); i += 4)
		{
			const unsigned char* current = (const unsigned char*) & src[i];
			if (current)
			{
				const uint16 index = (to_x + ((i/4)%w)) + (to_y + ((i/4)/w)) * w;
				pixels[index].r = current[0];
				pixels[index].g = current[1];
				pixels[index].b = current[2];
				pixels[index].a = current[3];
			}
		}
	}
};