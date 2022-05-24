#pragma once
#include "GameUIComponent.h"
class ImageComponent : public GameUIComponent
{
	friend FileIO;
public:
    ImageComponent(string identifier, CoolUUID uuid);
	ImageComponent(nlohmann::json& data, CoolUUID uuid);
	ImageComponent(ImageComponent const& other);

#if EDITOR
		void CreateEngineUI() override;
#endif

};

