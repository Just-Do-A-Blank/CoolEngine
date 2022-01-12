struct VS_INPUT
{
	float3 posL : POSITION;
	float2 texCoords : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 posH : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
    output.posH = float4(0, 0, 0, 1);
    return output;
}