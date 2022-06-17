
#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H

#include <d3d11.h>
#include <stdio.h>
#include <string>
#include <wrl.h>

namespace Engine
{
	namespace Shader
	{
		/**
		* @breif 頂点シェーダを生成するクラス
		*/
		class Vertex
		{
		public:
			/**
			* @breif コンストラクタ
			*/
			Vertex() = default;

			/**
			* @breif デストラクタ
			*/
			~Vertex()
			{
				p_VertexShader.Reset();
			}

		public:
			/**
			* @breif アクセサ関数
			* @return インターフェース
			*/
			ID3D11VertexShader* GetShaderInterface() const { return p_VertexShader.Get(); }

			/**
			* @breif アクセサ関数
			* @return データの開始位置
			*/
			inline char* GetData() const { return p_DataPoint; }

			/**
			* @breif アクセサ関数
			* @return データサイズ
			*/
			inline long GetSize() const { return p_DataSize; }

		private:
			/**
			* @breif シェーダファイル読み込み
			* @param p_FileName_ : 読みこむファイルの名前
			* @return データサイズ
			*/
			int LoadFile(const std::string& p_FileName_);

		public:
			/**
			* @breif シェーダ作成
			* @param p_Device_ : デバイス
			* @param p_FileName_ : 読みこむファイル
			* @return trueが帰ってきたら作成
			*/
			bool Create(ID3D11Device* p_Device_, const std::string& p_FileName_);

		private:
			Microsoft::WRL::ComPtr<ID3D11VertexShader> p_VertexShader { nullptr };	//! VertexShader
			char* p_DataPoint { nullptr };											//! データ開始位置
			long p_DataSize { 0 };													//! データサイズ
		};
	};
};

#endif // !VERTEX_SHADER_H
