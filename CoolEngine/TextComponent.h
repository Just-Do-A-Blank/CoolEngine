#pragma once
#include "Engine/GameUI/GameUIComponent.h"
class TextComponent :
    public GameUIComponent
{
private:
    float m_boxWidth;
    float m_textSize;
    XMFLOAT3 m_textColour;

    bool m_textWrap;
};

