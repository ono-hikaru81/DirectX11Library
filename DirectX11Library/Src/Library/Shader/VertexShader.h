
#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H

#include <d3d11.h>

#include "ShaderBase.h"

namespace Engine
{
	namespace Shader
	{
		/**
		* @breif 頂点シェーダを生成するクラス
		*/
		class Vertex : public Base
		{
		public:
			/**
			* @breif コンストラクタ
			*/
			Vertex() :
				p_VertexShader(nullptr) {}

			/**
			* @breif デストラクタ
			*/
			virtual ~Vertex()
			{
				if (p_VertexShader != nullptr)
				{
					p_VertexShader->Release();
				}
			}

		public:
			/**
			* @breif アクセサ関数
			* @return インターフェース
			*/
			ID3D11VertexShader* GetShaderInterface() { return p_VertexShader; }

		public:
			/**
			* @breif シェーダ作成
			* @param p_Device_ : デバイス
			* @param p_FileName_ : 読みこむファイル
			* @return trueが帰ってきたら作成
			*/
			virtual bool Create(ID3D11Device* p_Device_, const char* p_FileName_);

		private:
			ID3D11VertexShader* p_VertexShader; //! VertexShader
		};
	};
};

#endif // !VERTEX_SHADER_H
