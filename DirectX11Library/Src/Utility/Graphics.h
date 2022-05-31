
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <DirectXMath.h>

namespace Utility
{
	struct Vertex
	{
		float pos[4];
		float col[4];
	};

	struct ConstantBuffer
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4 viewport;
	};

	struct TextureVertex
	{
		float pos[3];
		float color[4];
		float uv[2];
	};
}

#endif // !GRAPHICS_H
