
// PixelShaderに送られてくるデータ構造
struct PSInput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
};

float4 ps_main(PSInput input) : SV_Target0
{
	return input.color;
}
