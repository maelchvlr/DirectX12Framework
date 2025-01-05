Texture2D sceneTexture : register(t0); // Scene render target
SamplerState samplerState : register(s0);

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 distortion = input.normal.xy * 0.02;

    // Apply distortion to texCoords
    float2 distortedTexCoord = input.texCoord + distortion;

    // Blur kernel offsets
    float2 offsets[9] =
    {
        { -1.0, -1.0 },
        { 0.0, -1.0 },
        { 1.0, -1.0 },
        { -1.0, 0.0 },
        { 0.0, 0.0 },
        { 1.0, 0.0 },
        { -1.0, 1.0 },
        { 0.0, 1.0 },
        { 1.0, 1.0 }
    };

    float blurSize = 0.0015; // Blur strength
    float4 color = float4(0, 0, 0, 0);

    for (int i = 0; i < 9; i++)
    {
        float2 sampleCoord = distortedTexCoord + offsets[i] * blurSize;
        color += sceneTexture.Sample(samplerState, sampleCoord);
    }

    return color / 9.0; // Average the samples for blur
}
