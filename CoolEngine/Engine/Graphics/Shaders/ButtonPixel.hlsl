#define PRESSED 0
#define RELEASED 1
#define HOVERED 2

struct PS_INPUT
{
    float4 posH : SV_POSITION;
    float2 texCoords : TEXCOORD;
};

Texture2D pressedTex : register(t0);
Texture2D releasedTex : register(t1);
Texture2D hoveredTex : register(t2);

SamplerState samLinear : register(s0);

cbuffer perFrameCB : register(b0)
{
    float4x4 viewProjection;
}

cbuffer textPerInstanceCB : register(b1)
{
    float4x4 world;
    float3 colour;
    int textureType;
}

float alphaClipThreshold = 0.1f;

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 albedo = float4(1.0f, 1.0f, 1.0f, 1.0f);

    if (textureType == PRESSED)
    {
        albedo = pressedTex.Sample(samLinear, input.texCoords);
    }
    else if (textureType == RELEASED)
    {
        albedo = releasedTex.Sample(samLinear, input.texCoords);
    }
    else
    {
        albedo = hoveredTex.Sample(samLinear, input.texCoords);
    }

    //NOTE 0.1f = alphaclipping below 0.1f  
    if (albedo.a < 0.1f)
    {
        discard;
    }

    albedo.rgb *= colour;

    return albedo;
}