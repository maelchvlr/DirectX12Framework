

struct PS_INPUT  // from the VS_OUTPUT
{
    float4 position : SV_POSITION;
};

struct MaterialData
{
    float4 diffuseAlbedo;
};

// old method, visual studio wasn't setup for a hlsl 5.1 when i wrote this.
cbuffer gMaterialData : register(b2)
{
    MaterialData gMaterialData;
};

float4 main(PS_INPUT input) : SV_TARGET
{

    return gMaterialData.diffuseAlbedo;
    //input.color;
}