
#ifndef PIXEL_SHADER_H
#define PIXEL_SHADER_H

#include <d3d11.h>

#include "ShaderBase.h"

namespace Library
{
	namespace Shader
	{
		/**
		* @breif ピクセルシェーダクラス
		*/
		class Pixcel : public Base
		{
		public:
			/**
			* @breif コンストラクタ
			*/
			Pixcel() :
				p_PixelShader(nullptr) {}

			/**
			* @breif デストラクタ
			*/
			virtual ~Pixcel()
			{
				if (p_PixelShader != nullptr)
				{
					p_PixelShader->Release();
				}
			}

		public:
			/**
			* @breif アクセサ関数
			* @return インターフェース
			*/
			ID3D11PixelShader* GetShaderInterface() { return p_PixelShader; }

		public:
			/**
			* @breif ピクセルシェーダ作成
			* @param p_Device_ : デバイス
			* @param p_FileName_ : 読みこむファイル
			* @return trueが帰ってきたら作成
			*/
			virtual bool Create(ID3D11Device* p_Device_, const char* p_FileName_);

		private:
			ID3D11PixelShader* p_PixelShader;	//! PixelShader
		};
	};
};

#endif // !PIXEL_SHADER_H
