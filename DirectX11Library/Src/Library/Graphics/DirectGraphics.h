
#ifndef DIRECT_GRAPHICS_H
#define DIRECT_GRAPHICS_H

#include <d3d11.h>
#include <map>
#include <memory>
#include <string>
#include <wrl.h>

#include "../Shader/VertexShader.h"
#include "../Shader/PixelShader.h"
#include "../../Utility/Graphics.h"

namespace Engine
{
	/*
	* @breif DirectX11での描画関連を使うクラス
	*/
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
		/*
		* @breif アクセサ関数
		* @return p_DepthStencilView
		*/
		ID3D11DepthStencilView* GetDepthStencilView() const { return p_DepthStencilView.Get(); }

		/**
		* @brief アクセサ関数
		* @return p_Device
		*/
		ID3D11Device* GetDevice() const  { return p_Device.Get(); }

		/*
		* @breif アクセサ関数
		* @return p_DeviceContext
		*/
		ID3D11DeviceContext* GetDeviceContext() const { return p_DeviceContext.Get(); }

		/*
		* @breif アクセサ関数
		* @return p_RenderTargetView
		*/
		ID3D11RenderTargetView** GetRenderTargetViewAddress() { return p_RenderTargetView.GetAddressOf(); }

		/*
		* @breif アクセサ関数
		* @return p_RenderTargetView
		*/
		ID3D11SamplerState** GetSamplerStateAddress() { return p_SamplerState.GetAddressOf(); }

		/*
		* @breif アクセサ関数
		* @return viewPort
		*/
		D3D11_VIEWPORT* GetViewPort() { return &viewPort; }

		/*
		* @breif アクセサ関数
		* @return 2Dポリゴン用頂点シェーダ
		*/
		Shader::Vertex* GetPorigon2DVertexShader() const { return p_Porigon2DVertexShader.get(); }

		/*
		* @breif アクセサ関数
		* @return 2Dポリゴン用ピクセルシェーダ
		*/
		Shader::Pixcel* GetPorigon2DPixelShader() const { return p_Porigon2DPixelShader.get(); }

		/*
		* @breif アクセサ関数
		* @return テクスチャ用頂点シェーダ
		*/
		Shader::Vertex* GetTextureVertexShader() const { return p_TextureVertexShader.get(); }

		/*
		* @breif アクセサ関数
		* @return テクスチャ用ピクセルシェーダ
		*/
		Shader::Pixcel* GetTexturePixelShader() const { return p_TexturePixelShader.get(); }

		/**
		* @breif アクセサ関数
		* @return ObjFile用頂点シェーダ
		*/
		Shader::Vertex* GetObjFileVertexShader() const { return p_ObjFileVertexShader.get(); }

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

		/*
		* @breif DeviceContext設定
		*/
		void SetUpDeviceContext();

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
		Microsoft::WRL::ComPtr<ID3D11Texture2D> p_DepthStencilTexture { nullptr };		//! DepthStencilView
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> p_DepthStencilView { nullptr };	//! DepthStencilView
		Microsoft::WRL::ComPtr<ID3D11Device> p_Device { nullptr };						//! Device
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> p_DeviceContext { nullptr };		//! DeviceContext
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> p_RenderTargetView { nullptr };	//! RenderTargetView
		Microsoft::WRL::ComPtr<IDXGISwapChain> p_SwapChain { nullptr };					//! SwapChain
		Microsoft::WRL::ComPtr<ID3D11SamplerState> p_SamplerState { nullptr };			//! SamplerState
		D3D11_VIEWPORT viewPort {};														//! ViewPort

		std::unique_ptr<Shader::Vertex> p_Porigon2DVertexShader { nullptr };	//! 2Dポリゴン用頂点シェーダ
		std::unique_ptr<Shader::Pixcel> p_Porigon2DPixelShader { nullptr };		//! 2Dポリゴン用ピクセルシェーダ

		std::unique_ptr<Shader::Vertex> p_TextureVertexShader { nullptr };	//! テクスチャ用頂点シェーダ
		std::unique_ptr<Shader::Pixcel> p_TexturePixelShader { nullptr };	//! テクスチャ用ピクセルシェーダ

		std::unique_ptr<Shader::Vertex> p_ObjFileVertexShader { nullptr };			//! ObjFile用頂点シェーダ
		std::unique_ptr<Shader::Pixcel> p_ObjFilePixelShader { nullptr };			//! ObjFile用ピクセルシェーダ
	};
};

#endif // !DIRECT_GRAPHICS_H
