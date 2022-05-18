
#ifndef SHADER_BASE_H
#define SHADER_BASE_H

#include <d3d11.h>
#include <stdio.h>

namespace Library
{
	namespace Shader
	{
		/**
		* @breif シェーダの基底クラス
		*/
		class Base
		{
		public:
			/**
			* @breif コンストラクタ
			*/
			Base() :
				p_DataSize(0),
				p_DataStartPoint(nullptr) {}

			/**
			* @breif デストラクタ
			*/
			virtual ~Base()
			{
				if (p_DataStartPoint != nullptr)
				{
					delete[] p_DataStartPoint;
				}
			}

		public:
			/**
			* @breif アクセサ関数
			* @return データの開始位置
			*/
			inline const char* GetData() { return p_DataStartPoint; }

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
			* @breif シェーダ作成
			* @param p_Device_ : デバイス
			* @param p_FileName_ : 読みこむファイルの名前
			* @return 成否
			*/
			virtual bool Create(ID3D11Device* p_Device_, const char* p_FileName_);

		protected:
			//! シェーダインターフェース
			union ShaderInterface
			{
				ID3D11VertexShader* p_Vertex;	//! 頂点シェーダ
				ID3D11PixelShader* p_Pixel;		//! ピクセルシェーダ
			};

			char* p_DataStartPoint;	//! データの開始位置
			long p_DataSize;		//! データサイズ
		};
	};
};

#endif // !SHADER_BASE_H
