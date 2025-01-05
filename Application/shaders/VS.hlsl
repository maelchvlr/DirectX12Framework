struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};

struct VS_OUTPUT
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

struct ObjectData
{
	float4x4 transform;
};

// old method, visual studio wasn't setup for a hlsl 5.1 when i wrote this.
cbuffer gPassData : register(b0)
{
    PassData gPassData;
};

// actual method
ConstantBuffer<ObjectData> gObjectData : register(b1);



VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	
    float3 worldPos = input.position;
    worldPos = mul(gObjectData.transform, float4(worldPos, 1.0f)).xyz;
	
    output.position = mul(gPassData.viewProj, float4(worldPos, 1.0f));
	
	
    output.normal = mul(gObjectData.transform, float4(input.normal, 1.0f)).xyz;

	return output;
	
}


