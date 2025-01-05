

struct PS_INPUT  // from the VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};


struct LightData
{
    float3 position;
    float strength;
    float3 direction;
    float padding;
};

struct PassData
{
    float4x4 viewProj;
    LightData light;
};

// old method, visual studio wasn't setup for a hlsl 5.1 when i wrote this.
cbuffer gPassData : register(b0)
{
    PassData gPassData;
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
    float intensity;
    float4 color;
    
    intensity = dot(-(gPassData.light.direction), input.normal);
    
    if(intensity > 0.90)
    {
        color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else if(intensity > 0.50)
    {
        color = float4(0.6f, 0.6f, 0.6f, 1.0f);
    }
    else if(intensity > 0.25)
    {
        color = float4(0.4f, 0.4f, 0.4f, 1.0f);
    }
    else
    {
        color = float4(0.2f, 0.2f, 0.2f, 1.0f);
    }
        
    return color * gMaterialData.diffuseAlbedo;
	//input.color;
}