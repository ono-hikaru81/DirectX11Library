
#include "PixelShader.h"

namespace Engine
{
	namespace Shader
	{
		// シェーダ作成
		bool Pixcel::Create(ID3D11Device* p_Device_, const char* p_FileName_)
		{
			if (!Base::Create(p_Device_, p_FileName_)) return false;

			if (FAILED(p_Device_->CreatePixelShader(p_DataStartPoint, p_DataSize, nullptr, &p_PixelShader))) return false;

			return true;
		}
	};
};
