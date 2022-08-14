
struct VS_IN
{
	float4 pos : POSITION0;
	float4 nor : NORMAL0;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
};

cbuffer ConstantBuffer
{
	float4x4	World;				// ワールド変換行列
	float4x4	View;				// ビュー変換行列
	float4x4	Projection;			// 透視射影変換行列
	float4		LightVector;		// ライト方向
	float4		LightColor;			// ライトカラー
}

VS_OUT main(VS_IN input)
{
	VS_OUT output;

	// ワールド座標変換
	// ローカル座標
	output.pos = mul(input.pos, World);
	// ビュー座標変換行列
	output.pos = mul(output.pos, View);
	// プロジェクション座標変換行列
	output.pos = mul(output.pos, Projection);

	float4 normal;
	
	input.nor.w = 0.0f; // wが1.0f以外のピクセルに送られる際に w = 1 に変換されるので戻しておく

	// 頂点の法線にワールド行列を掛け合わせてワールド座標上での法線の向きに変換する
	normal = normalize((mul(input.nor, World).xyzw));

	// saturate => 引数で指定した値を0～1間での範囲に収める
	// dot => 内積計算
	float rad = saturate(dot(normal, LightVector));

	// ライトのカラー * 光のあたり加減
	output.col = LightColor * rad;
	output.col.a = 1.0f;

	return output;
}
