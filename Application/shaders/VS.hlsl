struct VS_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
};



struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


struct PassData
{
	float4x4 viewProj;
};

cbuffer gPassData : register(b0)
{
    PassData gPassData;
};



VS_OUTPUT main(VS_INPUT input)
{
	
	
	VS_OUTPUT output;
    output.position = mul(gPassData.viewProj, float4(input.position, 1.0f));
	output.color = input.color;

	return output;
	
}


