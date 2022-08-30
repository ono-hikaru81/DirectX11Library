
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <DirectXMath.h>

#include "Vector.h"

namespace Utility
{
	//! 2Dポリゴン
	struct Porygon2D
	{
		//! 定数バッファ
		struct ConstantBuffer
		{
			DirectX::XMFLOAT4X4 world;	//! ワールドマトリクス
			DirectX::XMFLOAT4 viewPort;	//! ビューポート
			DirectX::XMFLOAT4 color;	//! 色
		};

		//! 頂点情報
		struct Vertex
		{
			float pos[4];	//! 座標
			float col[4];	//! 色
		};
	};

	//! 2Dテクスチャ
	struct TextureVertex
	{
		float pos[3];		//! 座標
		float color[4];		//! 色
		float uv[2];		//! UV座標
	};

	//! オブジェファイル
	struct ObjFile
	{
		//! 定数バッファ
		struct ConstantBuffer
		{
			DirectX::XMFLOAT4X4 world;		//! ワールドマトリクス
			DirectX::XMFLOAT4X4 view;		//! ビューマトリクス
			DirectX::XMFLOAT4X4 projection;	//! プロジェクションマトリクス
			DirectX::XMFLOAT4 lightVector;	//! ライトベクトル
			DirectX::XMFLOAT4 lightColor;	//! ライトの色
		};

		//! 頂点情報
		struct Vertex
		{
			Vector pos;		//! 座標
			Vector normal;	//! 法線
		};
	};
}

#endif // !GRAPHICS_H
