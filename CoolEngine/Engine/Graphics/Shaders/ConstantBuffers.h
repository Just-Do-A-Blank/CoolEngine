#pragma once

#ifdef HLSL
#include "HlslCompat.h"
#else
#include <DirectXMath.h>

using namespace DirectX;
#endif

struct PerFrameCB
{
	XMMATRIX viewProjection;
};

struct PerInstanceCB
{
	XMMATRIX world;
};