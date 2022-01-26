#pragma once
#include "Engine/Structure/Singleton.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>

class FontManager : public Singleton<FontManager>
{
private:

	SpriteBatch* m_pspriteBatch;
	unordered_map<string, SpriteFont*> m_spriteFontMap;

public:
	void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	void Render();
};

