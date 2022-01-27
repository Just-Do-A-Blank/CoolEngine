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

struct DebugPerInstanceCB
{
	DirectX::XMFLOAT4X4 placeholder;
};

struct TextPerInstanceCB
{
	XMFLOAT4X4 world;
	XMFLOAT3 colour;
	float pad;
};