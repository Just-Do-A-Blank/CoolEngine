#pragma once

#include <DirectXMath.h>

struct PerFrameCB
{
	DirectX::XMFLOAT4X4 viewProjection;
};

struct PerInstanceCB
{
	DirectX::XMFLOAT4X4 world;
};