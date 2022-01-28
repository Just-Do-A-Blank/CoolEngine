#pragma once
#include "GameUIComponent.h"
class ImageComponent : public GameUIComponent
{
	friend FileIO;
public:
    ImageComponent(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation);
	void Init(wstring textureFilePath);

#if EDITOR
		void CreateEngineUI() override;
#endif

};

