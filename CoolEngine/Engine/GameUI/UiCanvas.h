#pragma once
#include "Engine/GameUI/GameUIComponent.h"


class UICanvas : public GameUIComponent
{
public:
	UICanvas(string identifier, CoolUUID uuid, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation);
	UICanvas(nlohmann::json& data, CoolUUID uuid);

	void Render(RenderStruct& renderStruct) override;
	void Update() override;
};

