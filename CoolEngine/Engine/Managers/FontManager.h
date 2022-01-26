#pragma once
#include "Engine/Structure/Singleton.h"

struct FontAtlasStruct
{
	int charId;
	XMFLOAT2 charTexPosition;
	XMFLOAT2 charDimension;
};

class FontManager : public Singleton<FontManager>
{
private:

public:
	void LoadFonts();
};

