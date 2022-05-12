
struct VS_INPUT
{
	float3 posW : SV_Position;
	float4 color : COLOR;
	float2 texCoords : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 posH : SV_POSITION;	
	float2 texCoords : TEXCOORD;
};

cbuffer perFrameCB : register(b0)
{
    float4x4 viewProjection;
}

cbuffer perInstanceCB : register(b1)
{
    float4x4 world;
}

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
    output.posH = mul(float4(input.posW, 1), viewProjection);
	output.texCoords = input.texCoords;

	return output;
}