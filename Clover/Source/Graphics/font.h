#pragma once

#ifndef _FONT_H
#define _FONT_H

#include <Clover.h>
#include <map>
#include <Graphics/texture.h>


struct Character 
{
	Ref<SubTexture2D> CharTexture;
	glm::ivec2 Size;
};


class Font
{
public:
	Font(const std::string& FontFile, const glm::vec2 FontSize, const glm::vec2 GlyphSize);

	std::map<char, Character> Characters;
	Ref<Texture2D> FontTexture;
};

#endif