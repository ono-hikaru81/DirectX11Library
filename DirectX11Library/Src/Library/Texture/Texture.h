
#ifndef TEXTURE_H
#define TEXTURE_H

#include <d3d11.h>
#include <wrl.h>

#include "../Shader/VertexShader.h"

namespace Library
{
	/*
	* @breif テクスチャを描画する為のクラス
	*/
	class Texture
	{
	public:
		/*
		* @breif コンストラクタ
		*/
		Texture() :
			p_VertexBuffer(nullptr),
			p_InputLayout(nullptr)
		{}

		/*
		* @breif デストラクタ
		*/
		~Texture()
		{
			if (p_VertexBuffer != nullptr)
			{
				p_VertexBuffer->Release();
				p_VertexBuffer = nullptr;
			}

			if (p_InputLayout != nullptr)
			{
				p_InputLayout->Release();
				p_InputLayout = nullptr;
			}
		}

	public:
		/*
		* @breif アクセサ関数
		* @return 頂点バッファ
		*/
		ID3D11Buffer* GetVertexBuffer() { return p_VertexBuffer.Get(); }
		/*
		* @breif アクセサ関数
		* @return 入力レイアウト
		*/
		ID3D11InputLayout* GetInputLayput() { return p_InputLayout.Get(); }

	public:
		/*
		* @breif テクスチャを張り付けるポリゴンを作成
		* @param p_Device : デバイス
		* @param p_VertexShader : 頂点シェーダー
		*/
		bool CreatePorigonModel(ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_);

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> p_VertexBuffer;		//! 頂点バッファ
		Microsoft::WRL::ComPtr<ID3D11InputLayout> p_InputLayout;	//! 入力レイアウト
	};
}

#endif // !TEXTURE_H
