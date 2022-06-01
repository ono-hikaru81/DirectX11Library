
#ifndef MODEL_DATA_H
#define MODEL_DATA_H

#include <d3d11.h>

#include "../Shader/VertexShader.h"

namespace Engine
{
	class PolygonData
	{
	public:
		/**
		* @breif コンストラクタ
		*/
		PolygonData() :
			p_VertexBuffer(nullptr),
			p_IndexBuffer(nullptr),
			p_InputLayout(nullptr)
		{}

		/**
		* @breif デストラクタ
		*/
		~PolygonData()
		{
			if (p_VertexBuffer != nullptr)
			{
				p_VertexBuffer->Release();
			}

			if (p_IndexBuffer != nullptr)
			{
				p_IndexBuffer->Release();
			}

			if (p_InputLayout != nullptr)
			{
				p_InputLayout->Release();
			}
		}

	public:
		/**
		* @breif アクセサ関数
		* @return 頂点バッファ
		*/
		ID3D11Buffer* GetVertexBuffer() { return p_VertexBuffer; }
		/**
		* @breif アクセサ関数
		* @return インデックスバッファ
		*/
		ID3D11Buffer* GetIndexBuffer() { return p_IndexBuffer; }
		/**
		* @breif アクセサ関数
		* @return 入力レイアウト
		*/
		ID3D11InputLayout* GetInputLayout() { return p_InputLayout; }

	public:
		/**
		* @breif ポリゴン作成
		* @param p_Device_ : デバイス
		* @param p_VertexShader_ : 頂点シェーダ
		* @return trueなら作成
		*/
		bool CreatePorigon(ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_);
		/**
		* @breif 矩形作成
		* @param p_Device_ : デバイス
		* @param p_VertexShader_ : 頂点シェーダ
		* @return trueなら作成
		*/
		bool CreateRect(ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_);

	private:
		ID3D11Buffer* p_VertexBuffer;		//! 頂点バッファ
		ID3D11Buffer* p_IndexBuffer;		//! インデックスバッファ
		ID3D11InputLayout* p_InputLayout;	//! 入力レイアウト
	};
};

#endif // !MODEL_DATA_H
