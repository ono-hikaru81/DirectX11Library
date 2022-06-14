
#ifndef DIRECT_GRAPHICS_H
#define DIRECT_GRAPHICS_H

#include <d3d11.h>
#include <map>
#include <memory>
#include <string>
#include <wrl.h>

#include "../Shader/VertexShader.h"
#include "../Shader/PixelShader.h"
#include "../Polygon/PolygonData.h"
#include "../Texture/Texture.h"
#include "../../Utility/Graphics.h"

namespace Engine
{
	class DirectGraphics
	{
	public:
		/**
		* @breif コンストラクタ
		*/
		DirectGraphics() = default;

		/**
		* @breif デストラクタ
		*/
		~DirectGraphics() = default;

	public:
		/**
		* @brief アクセサ関数
		* @return デバイス
		*/
		ID3D11Device* GetDevice() { return p_Device.Get(); }

		/*
		* @breif アクセサ関数
		* @return デバイスコンテキスト
		*/
		ID3D11DeviceContext* GetDeviceContext() { return p_DeviceContext.Get(); }

		/**
		* @breif アクセサ関数
		* @return 頂点シェーダ
		*/
		Shader::Vertex* GetVertexShader() { return p_ObjFileVertexShader.get(); }

		/*
		* @breif アクセサ関数
		* @return 
		*/
		ID3D11Buffer* GetObjFileConstantBuffer() { return p_ObjFileConstantBuffer; }

		/*
		*/
		Utility::ObjFileConstantBuffer* GetObjFileConstantBufferData() { return &objFileConstantBufferData; }

	public:
		/**
		* @breif 初期化
		* @return trueなら初期化成功
		*/
		bool Initialize();

		/**
		* @breif 解放
		*/
		void Release();

		/**
		* @breif 描画開始
		*/
		void StartRendering();

		/**
		* @breif 描画終了
		*/
		void FinishRendering();

		void SetUpTransform();

		void SetUpDeviceContext();

		/**
		* @breif ポリゴン描画
		* @param posX_ : 左上X座標
		* @param posY_ : 左上Y座標
		* @param width_ : X軸の大きさ
		* @param height_ : Y軸の大きさ
		* @param angle_ : 角度
		*/
		void DrawPorigon(float posX_, float posY_, float width_, float height_, float angle_ = 0.0f);

		/**
		* @breif 矩形描画
		* @param posX_ : 左上X座標
		* @param posY_ : 左上Y座標
		* @param width_ : X軸の大きさ
		* @param height_ : Y軸の大きさ
		* @param angle_ : 角度
		*/
		void DrawRect(float posX_, float posY_, float width_, float height_, float angle_ = 0.0f);

		/*
		* @brief テクスチャ読み込み
		* @return trueなら読み込み成功
		* @param fileName_ : 読みこむテクスチャファイルの名前
		*/
		bool LoadTexture(const std::wstring fileName_);

		/*
		* @breif テクスチャ解放
		* @param fileName_ : 解放するテクスチャファイルの名前
		*/
		void ReleaseTexture(const std::wstring fileName_);

		/*
		* @breif 全てのテクスチャを解放
		* 
		*/
		void ReleaseAllTexture();

		/*
		* @breif テクスチャ描画
		* @param fileName_ : 読みこむテクスチャファイルの名前
		*/
		void DrawTexture(const std::wstring fileName_, float posX_, float posY_, float width_, float height_, float angle_ = 0.0f);

	private:
		/**
		* @breif DXGI_SWAP_CHAIN_DESCの設定関数
		* @param p_Dxgi_ : 設定するDXGI_SWAP_CHAIN_DESCのポインタ
		*/
		void SetUpDxgiSwapChainDesc(DXGI_SWAP_CHAIN_DESC* p_Dxgi_);

		/**
		* @breif DeviceとSwapChainの作成関数
		* @return trueなら作成
		*/
		bool CreateDeviceAndSwapChain();

		/**
		* @breif RenderTargetViewの作成関数
		* @return trueなら作成
		*/
		bool CreateRenderTargetView();

		/**
		* @breif DepthバッファとStencilバッファの作成関数
		* @return trueなら作成
		*/
		bool CreateDepthAndStencilView();

		/**
		* @breif ConstantBufferの作成関数
		* @return trueなら作成
		*/
		bool CreateConstantBuffer();

		/*
		* @breif ObjFile用ConstantBufferの作成関数
		* @return trueなら作成
		*/
		bool CreateObjFileConstantBuffer();

		/**
		* @breif シェーダの作成関数
		* @return trueなら作成
		*/
		bool CreateShader();

		/**
		* @breif ViewPortの設定
		* @return trueなら作成
		*/
		void SetUpViewPort();

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> p_Device { nullptr };					//! Device
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> p_DeviceContext { nullptr };	//! DeviceContext
		Microsoft::WRL::ComPtr<IDXGISwapChain> p_SwapChain { nullptr };				//! SwapChain
		ID3D11RenderTargetView* p_RenderTargetView { nullptr };						//! RenderTargetView
		ID3D11Texture2D* p_DepthStencilTexture { nullptr };							//! DepthStencilView
		ID3D11DepthStencilView* p_DepthStencilView { nullptr };						//! DepthStencilView
		D3D11_VIEWPORT viewPort {};													//! ViewPort
		ID3D11SamplerState* p_SamplerState { nullptr };								//! SamplerState

		Shader::Vertex* p_2DPorigonVertexShader { nullptr };	//! 2Dポリゴン用頂点シェーダ
		Shader::Pixcel* p_2DPorigonPixelShader { nullptr };		//! 2Dポリゴン用ピクセルシェーダ
		ID3D11Buffer* p_ConstantBuffer{ nullptr };				//! 定数Buffer
		Utility::ConstantBuffer constantBufferData {};			//! ConstantBuffer
		PolygonData* p_Porigon { nullptr };						//! 三角形のデータ
		PolygonData* p_Rect { nullptr };						//! 矩形のデータ

		Shader::Vertex* p_TextureVertexShader { nullptr };					//! テクスチャ用頂点シェーダ
		Shader::Pixcel* p_TexturePixelShader { nullptr };					//! テクスチャ用ピクセルシェーダ
		Texture* p_Texture { nullptr };										//! テクスチャ管理
		std::map<std::wstring, ID3D11ShaderResourceView*> textureList {};	//! テクスチャリスト

		std::unique_ptr<Shader::Vertex> p_ObjFileVertexShader { nullptr };	//! オブジェファイル用頂点シェーダ
		Shader::Pixcel* p_ObjFilePixelShader { nullptr };					//! オブジェファイル用ピクセルシェーダ
		ID3D11Buffer* p_ObjFileConstantBuffer{ nullptr };					//! オブジェファイル用定数バッファ
		Utility::ObjFileConstantBuffer objFileConstantBufferData {};		//! オブジェファイル用定数バッファデータ
	};
};

#endif // !DIRECT_GRAPHICS_H
