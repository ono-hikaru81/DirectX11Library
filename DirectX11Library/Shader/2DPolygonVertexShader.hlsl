
cbuffer ConstantBuffer
{
	float4x4 worldMatrix;		// ワールド変換行列
	float4 viewPort;			// ビューポート
}

// VertexShaderに送られてくるデータ構造
struct VSInput
{
	float4 pos : POSITION0;
	float4 color : COLOR0;
};

// VertexShaderから出力するデータ構造
struct VSOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
};

VSOutput vs_main(VSInput input)
{
	VSOutput output;

	output.pos = input.pos;
	output.color = input.color;

	float4x4 world = worldMatrix;

	// ワールド座標とスクリーン座標ではY軸が異なるため移動方向を反転
	world[3][1] *= -1.0f;

	//// 移動量も 0～1 の範囲ではなく 0～2 の範囲で移動させる
	world[3][0] *= 2.0f;
	world[3][1] *= 2.0f;

	///*
	//	頂点座標に 2.0 を掛けているのは
	//	頂点の範囲が -0.5～0.5 の幅1の範囲にとどまっているため
	//	ビューポート範囲(-1～1)に合わせているから
	//*/
	float4 localPos = input.pos;
	localPos *= 2.0f;
	localPos.w = 1.0f; // wが1.0f以外のピクセルに送られる際に w = 1 に変換されるので戻しておく

	//// ワールド座標変換
	output.pos = mul(localPos, world);

	//// 座標をビューポートのサイズで割ってプロジェクション空間(-1～1)に合わせる
	float projectionPosX = output.pos.x / viewPort.x;
	float projectionPosY = output.pos.y / viewPort.y;

	//// 原点を画面中央から左上へと変換するオフセット値
	float offsetX = -1.0f - (-abs(localPos.x) * world[0][0]) / viewPort.x;
	float offsetY = 1.0f - (abs(localPos.y) * world[1][1]) / viewPort.y;

	//// オフセットを足して原点の位置を修正
	output.pos.x = projectionPosX + offsetX;
	output.pos.y = projectionPosY + offsetY;

	return output;
}
