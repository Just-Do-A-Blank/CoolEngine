struct VS_INPUT
{
	float3 posL : POSITION;
	float2 texCoords : TEXCOORD;
    uint id : TEXCOORD1;
};

struct VS_OUTPUT
{
	float4 posH : SV_POSITION;
    uint id : TEXCOORD1;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
    output.posH = float4(0, 0, 0, 1);
    output.id = input.id;
    return output;
}