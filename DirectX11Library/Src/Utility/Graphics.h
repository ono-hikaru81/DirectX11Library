
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
			DirectX::XMFLOAT4X4 world;
			DirectX::XMFLOAT4 viewPort;
		};

		//! 頂点情報
		struct Vertex
		{
			float pos[4];
			float col[4];
		};
	};

	//! 2Dテクスチャ
	struct TextureVertex
	{
		float pos[3];
		float color[4];
		float uv[2];
	};

	//! オブジェファイル
	struct ObjFile
	{
		//! 定数バッファ
		struct ConstantBuffer
		{
			DirectX::XMFLOAT4X4 world;
			DirectX::XMFLOAT4X4 view;
			DirectX::XMFLOAT4X4 projection;
			DirectX::XMFLOAT4 lightVector;
			DirectX::XMFLOAT4 lightColor;
		};

		//! 頂点情報
		struct Vertex
		{
			Vector pos;
			Vector normal;
		};
	};
}

#endif // !GRAPHICS_H
