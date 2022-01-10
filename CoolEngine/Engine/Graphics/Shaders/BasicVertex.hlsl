#define HLSL
#include "../ConstantBuffers.h"

struct VS_INPUT
{
	float3 posL : POSITION;
	float2 texCoords : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 posH : SV_POSITION;
	float2 texCoords : TEXCOORD;
};

float4 main(VS_INPUT input) : SV_POSITION
{
	VS_OUTPUT output;
	output.posH = mul(float4(input, 1.0f), world);
	output.texCoords = input.texCoords;

	return output;
}