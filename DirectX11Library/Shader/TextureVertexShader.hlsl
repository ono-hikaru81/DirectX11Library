
struct VSInput
{
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 UV : TEXCOORD; // テクスチャのUV値
};

struct VSOutput
{
	float4 pos : SV_POSITION;
	float4 color : TEXCOORD0;
	float2 UV : TEXCOORD1; // テクスチャのUV値
};

VSOutput vs_main(VSInput input)
{
	VSOutput output;

	output.pos = input.pos;
	output.color = input.color;
	output.UV = input.UV;

	return output;
}
