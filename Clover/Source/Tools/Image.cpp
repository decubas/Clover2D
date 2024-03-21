#include "image.h"
#include "stb_image.h"
#include "stb_image_resize.h"

Image::Image(const char* file)
{
	int w, h, channels;
	stbi_uc* data = stbi_load(file, &w, &h, &channels, 0);

	width = w;
	height = h;
	pixels = new Pixel[w * h];
	int32 j = 0;
	for (int i = 0; i < (w * h * channels); i += channels)
	{
		stbi_uc* current = &data[i];
		if (current)
		{
			pixels[j].r = current[0];
			pixels[j].g = current[1];
			pixels[j].b = current[2];
			pixels[j].a = channels == 4 ? current[3] : 0xFF;
			j++;
		}
	}

	stbi_image_free(data);
}

void Image::Resize(const uint16 w, const uint16 h)
{
	unsigned char* ResizedImage = (unsigned char*)malloc(w * h * 4);
	stbir_resize_uint8((const unsigned char*)pixels, width, height, 0, ResizedImage, w, h, 0, 4);

	delete[] pixels;
	
	Create(w, h);

	DrawImageFromSource(ResizedImage, w, h, 0, 0);

	free(ResizedImage);
}
