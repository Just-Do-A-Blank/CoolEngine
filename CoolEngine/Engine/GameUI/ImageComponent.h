#pragma once
#include "GameUIComponent.h"
class ImageComponent : public GameUIComponent
{
	friend FileIO;
public:
    ImageComponent(string identifier, CoolUUID uuid, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation);
	ImageComponent(nlohmann::json& data, CoolUUID uuid);

#if EDITOR
		void CreateEngineUI() override;
#endif

};

