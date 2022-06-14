
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <DirectXMath.h>

#include "Vector.h"

namespace Utility
{
	//==============
	// 2Dポリゴン用
	//==============
	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4 viewPort;
	};

	struct Vertex
	{
		float pos[4];
		float col[4];
	};

	//==============
	// テクスチャ用
	//==============
	struct TextureVertex
	{
		float pos[3];
		float color[4];
		float uv[2];
	};

	//==========
	// モデル用
	//==========
	struct ObjFileConstantBuffer
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
		DirectX::XMFLOAT4 lightVector;
		DirectX::XMFLOAT4 lightColor;
	};

	struct CustomVertex
	{
		Vector pos;
		Vector normal;
	};
}

#endif // !GRAPHICS_H
