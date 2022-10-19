
#include "font.h"
#include "graphiccontext.h"

Font::Font(const std::string& FontFile, const glm::vec2 FontSize, const glm::vec2 GlyphSize)
{
	FontTexture = GraphicContext::CreateTexture2D(FontFile);
	FontTexture->setMagFilter(Texture::Filter::NEAREST);

	unsigned char c = 32;
	float sizex = FontSize.x / GlyphSize.x;
	float sizey = FontSize.y / GlyphSize.y;
	for (float y = 0; y < FontSize.y; y++)
	{
		for (float x = 0; x < FontSize.x; x++)
		{
			Character ch;
			ch.CharTexture = GraphicContext::CreateSubTexture2D(FontTexture, { x, y }, GlyphSize);
			ch.Size = GlyphSize;
			Characters.insert(std::pair<char, Character>(c, ch));
			c++;
		}
	}
}
