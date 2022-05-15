#pragma once
#include "Engine/GameUI/GameUIComponent.h"


class UICanvas : public GameUIComponent
{
public:
	UICanvas(string identifier, CoolUUID uuid);
	UICanvas(nlohmann::json& data, CoolUUID uuid);

	void Render(RenderStruct& renderStruct) override;
	void Update() override;
};

