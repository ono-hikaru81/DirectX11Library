
#ifndef PIXEL_SHADER_H
#define PIXEL_SHADER_H

#include <d3d11.h>
#include <stdio.h>
#include <wrl.h>

namespace Engine
{
	namespace Shader
	{
		/**
		* @breif ピクセルシェーダを生成するクラス
		*/
		class Pixcel
		{
		public:
			/**
			* @breif コンストラクタ
			*/
			Pixcel() = default;

			/**
			* @breif デストラクタ
			*/
			~Pixcel()
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
			ID3D11PixelShader* GetShaderInterface() { return p_PixelShader.Get(); }

			/**
			* @breif アクセサ関数
			* @return データの開始位置
			*/
			inline const char* GetData() { return p_DataPoint; }

			/**
			* @breif アクセサ関数
			* @return データサイズ
			*/
			inline long GetSize() { return p_DataSize; }

		private:
			/**
			* @breif シェーダファイル読み込み
			* @param p_FileName_ : 読みこむファイルの名前
			* @return データサイズ
			*/
			int LoadFile(const char* p_FileName_);

		public:
			/**
			* @breif ピクセルシェーダ作成
			* @param p_Device_ : デバイス
			* @param p_FileName_ : 読みこむファイル
			* @return trueが帰ってきたら作成
			*/
			bool Create(ID3D11Device* p_Device_, const char* p_FileName_);

		private:
			Microsoft::WRL::ComPtr<ID3D11PixelShader> p_PixelShader { nullptr };	//! PixelShader
			char* p_DataPoint{ nullptr };											//! データ開始位置
			long p_DataSize{ 0 };													//! データサイズ
		};
	};
};

#endif // !PIXEL_SHADER_H
