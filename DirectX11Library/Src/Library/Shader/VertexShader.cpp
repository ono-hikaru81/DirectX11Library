
#include "VertexShader.h"

namespace Library
{
	namespace Shader
	{
		// シェーダ作成
		bool Vertex::Create(ID3D11Device* p_Device_, const char* p_FileName_)
		{
			if (!Base::Create(p_Device_, p_FileName_)) return false;

			if (FAILED(p_Device_->CreateVertexShader(p_DataStartPoint, p_DataSize, nullptr, &p_VertexShader))) return false;

			return true;
		}
	};
};
