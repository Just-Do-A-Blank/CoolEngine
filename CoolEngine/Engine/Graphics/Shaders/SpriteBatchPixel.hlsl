
struct PS_INPUT
{
    float4 posH : SV_POSITION;
    float4 color : COLOR;
    float2 texCoords : TEXCOORD;
};

Texture2D<float4> Texture : register(t0);
sampler TextureSampler : register(s0);

cbuffer perFrameCB : register(b0)
{
    float4x4 viewProjection;
}

cbuffer perInstanceCB : register(b1)
{
    float4x4 world;
}

static float alphaClipThreshold = 0.5f;

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 albedo = Texture.Sample(TextureSampler, input.texCoords);

    if (albedo.a < alphaClipThreshold)
    {
        discard;
    }

    return albedo * input.color;
}