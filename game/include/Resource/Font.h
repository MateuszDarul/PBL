#pragma once

#include <string>
#include <map>

#include <glm/matrix.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character
{
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class Font
{
public:
	Font();
	~Font();

	bool Load(const std::string& pathToFont, FT_Library& ft);

	std::map<char, Character> characters;
};