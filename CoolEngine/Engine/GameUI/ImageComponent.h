#pragma once
#include "GameUIComponent.h"
class ImageComponent : public GameUIComponent
{

public:
    ImageComponent(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation);

    string& GetIdentifier();

};

