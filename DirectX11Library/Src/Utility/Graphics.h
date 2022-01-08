
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
		DirectX::XMFLOAT4 viewPort;
	};
}

#endif // !GRAPHICS_H
