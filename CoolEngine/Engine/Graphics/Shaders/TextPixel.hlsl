
struct PS_INPUT
{
	float4 posH : SV_POSITION;
	float2 texCoords : TEXCOORD;
};

Texture2D albedoTex : register(t0);

SamplerState samLinear : register(s0);

cbuffer perFrameCB : register(b0)
{
    float4x4 viewProjection;
}

cbuffer textPerInstanceCB : register(b1)
{
    float4x4 world;
    float3 colour;
    float pad;
}

float alphaClipThreshold = 0.1f;

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 albedo = albedoTex.Sample(samLinear, input.texCoords);

    if (albedo.r == 0)
    {
        discard;
    }
    
    albedo.rgb *= colour;

    return albedo;
}