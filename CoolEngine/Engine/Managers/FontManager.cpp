#include "FontManager.h"

void FontManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	m_pspriteBatch = new SpriteBatch(pDeviceContext);
	SpriteFont* spriteFont = new SpriteFont(pDevice, L"Engine/Fonts/comicSansMS_32");

	m_spriteFontMap.insert(pair<string, SpriteFont*>("comicSansMS_32", spriteFont));
}

void FontManager::Render()
{
	/*m_pspriteBatch->Begin();
	m_spriteFontMap["test"]->FindGlyph(L'comicSansMS_32');
	m_pspriteBatch->End();*/
}
