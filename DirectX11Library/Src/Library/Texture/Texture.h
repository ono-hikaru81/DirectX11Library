
#ifndef TEXTURE_H
#define TEXTURE_H

#include <d3d11.h>
#include <map>
#include <string>
#include <wrl.h>

#include "../Graphics/DirectGraphics.h"
#include "../Shader/VertexShader.h"

namespace Engine
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
		Texture() = default;

		/*
		* @breif デストラクタ
		*/
		~Texture() = default;

	public:
		/**
		* @breif 初期化
		* @return trueなら初期化成功
		*/
		bool Initialize(DirectGraphics* p_DirectGraphics_);

		/**
		* @breif 解放
		*/
		void Release();

		/*
		* @brief テクスチャ読み込み
		* @param fileName_ : 読みこむテクスチャファイルの名前
		* @return trueなら読み込み成功
		*/
		bool LoadTexture(DirectGraphics* p_DirectGraphics_, const std::wstring& fileName_);

		/*
		* @breif テクスチャ解放
		* @param fileName_ : 解放するテクスチャファイルの名前
		*/
		void ReleaseTexture(const std::wstring& fileName_);

		/*
		* @breif 全てのテクスチャを解放
		*/
		void ReleaseAllTexture();

		/*
		* @breif テクスチャ描画
		* @param fileName_ : 読みこむテクスチャファイルの名前
		* @param posX_ : 左上X座標
		* @param posY_ : 左上Y座標
		* @param width_ : X軸の大きさ
		* @param height_ : Y軸の大きさ
		* @param angle_ : 角度
		*/
		void DrawTexture(DirectGraphics* p_DirectGraphics_, const std::wstring& fileName_, const float& posX_, const float& posY_, const float& width_, const float& height_, const float angle_ = 0.0f);

	private:
		/*
		* @breif テクスチャを張り付けるポリゴンを作成
		* @param p_Device : デバイス
		* @param p_VertexShader : 頂点シェーダー
		*/
		bool CreatePolygonModel(ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_);

		/**
		* @breif 定数バッファの作成
		* @return trueなら作成
		*/
		bool CreateConstantBuffer(DirectGraphics* p_DirectGraphics_);

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> p_VertexBuffer { nullptr };		//! 頂点バッファ
		Microsoft::WRL::ComPtr <ID3D11Buffer> p_IndexBuffer { nullptr };		//! インデックスバッファ
		Microsoft::WRL::ComPtr<ID3D11InputLayout> p_InputLayout { nullptr };	//! 入力レイアウト
		std::map<std::wstring, ID3D11ShaderResourceView*> textureList {};		//! テクスチャリスト
		Microsoft::WRL::ComPtr<ID3D11Buffer> p_ConstantBuffer { nullptr };		//! 定数バッファ
		Utility::Porygon2D::ConstantBuffer constantBufferData {};				//! 定数バッファのデータ
	};
}

#endif // !TEXTURE_H
