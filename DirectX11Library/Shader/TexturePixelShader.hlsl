
Texture2D diffuse : register(t0);
SamplerState samplerDiffuse : register(s0);

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 UV : TEXCOORD0; // テクスチャのUV値
};

float4 ps_main(PSInput input) : SV_Target0
{
	return diffuse.Sample(samplerDiffuse, input.UV);
}