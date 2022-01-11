#define HLSL
#include "ConstantBuffers.h"

struct VS_INPUT
{
	float3 posL : POSITION;
	float2 texCoords : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 posH : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input) : SV_POSITION
{
	VS_OUTPUT output;
    return output;
}