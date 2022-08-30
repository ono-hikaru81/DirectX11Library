
cbuffer ConstantBuffer
{
	float4x4 worldMatrix;	// ワールド変換行列
	float4 viewPort;		// ビューポート
	float4 color;			// 色
}

// VertexShaderに送られてくるデータ構造
struct VSInput
{
	float4 pos : POSITION0;	// 座標
	float4 color : COLOR0;	// 色
};

// VertexShaderから出力するデータ構造
struct VSOutput
{
	float4 pos : SV_POSITION;	// 座標
	float4 color : COLOR0;		// 色
};

VSOutput main(VSInput input)
{
	VSOutput output = input;
	float4x4 world = worldMatrix;

	output.color = color;

	// Y軸の移動方向を反転し、移動量も2倍にしておく
	world[3][0] *= 2.0f;
	world[3][1] *= -2.0f;

	// ビューポート範囲に合わせるために頂点座標を2倍にする
	float4 localPos = input.pos;
	localPos *= 2.0f;
	localPos.w = 1.0f; // wが1.0f以外のピクセルに送られる際に w = 1 に変換されるので戻しておく

	// ワールド座標変換
	output.pos = mul(localPos, world);

	// 座標をビューポートのサイズで割ってプロジェクション空間(-1～1)に合わせる
	float projectionPosX = output.pos.x / viewPort.x;
	float projectionPosY = output.pos.y / viewPort.y;

	// 原点を画面中央から左上へと変換するオフセット値
	float offsetX = -1.0f - (-abs(localPos.x) * world[0][0]) / viewPort.x;
	float offsetY = 1.0f - (abs(localPos.y) * world[1][1]) / viewPort.y;

	// オフセットを足して原点の位置を修正
	output.pos.x = projectionPosX + offsetX;
	output.pos.y = projectionPosY + offsetY;

	return output;
}
