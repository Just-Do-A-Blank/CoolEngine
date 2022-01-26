#pragma once
#include "GameUIComponent.h"
#include <DirectXTK-main/Inc/SpriteBatch.h>
#include <DirectXTK-main/Inc/SpriteFont.h>

class TextComponent : public GameUIComponent
{
private:
	SpriteBatch* m_pspriteBatch;
	SpriteFont* m_pspriteFont;

protected:

public:
	void InitializeFont();

};

