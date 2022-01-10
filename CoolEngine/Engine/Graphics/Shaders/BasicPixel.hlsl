#define HLSL
#include "../ConstantBuffers.h"

struct PS_INPUT
{
	float4 posH : SV_POSITION;
	float2 texCoords : TEXCOORD;
};

Texture2D albedoTex : register(t0);

SamplerState samLinear : register(s0);

float alphaClipThreshold = 0.1f;

float4 main() : SV_TARGET
{
	float4 albedo = albedoTex.Sample(samLinear, input.texCoords);

	if (albedo.a < alphaClipThreshold)
	{
		discard;
	}

	return albedo;
}