
#ifndef DIRECT_GRAPHICS_H
#define DIRECT_GRAPHICS_H

#include <d3d11.h>

#include "Shader/VertexShader.h"
#include "Shader/PixelShader.h"
#include "../Engine/PolygonData.h"
#include "../Utility/Graphics.h"

namespace DirectX
{
	class Graphics
	{
	public:
		Graphics() = default;
		~Graphics() = default;


	public:
		ID3D11Device* GetDevice() { return p_Device; }

		shader::Vertex* GetVertexShader() { return p_VertexShader; }

	public:

		// 初期化関数
		// DirectX11の初期化を行う。成功したらtrue、失敗したらfalseを返す
		bool Init();

		// 解放関数
		// 保持しているデバイスなどを解放する
		void Release();

		// 描画開始関数
		// 描画処理を行う場合、必ずこの関数の後に実行する
		void StartRendering();

		// 描画終了関数
		// 描画処理が完了した場合、必ずこの関数を実行する
		void FinishRendering();

		// ポリゴン描画関数
		void DrawPorigon(float posX_, float posY_, float width_, float height_, float angle_ = 0.0f);

		// 矩形描画関数
		void DrawRect(float posX_, float posY_, float width_, float height_, float angle_ = 0.0f);

		// 立方体描画関数
		void DrawCube(float posX_, float posY_, float width_, float height_, float angle_ = 0.0f);

	private:
		// DXGI_SWAP_CHAIN_DESCの設定関数
		// SwapChainを作成するうえで必要な設定をDXGI_SWAP_CHAIN_DESCに行う
		// dxgi 設定を行うDXGI_SWAP_CHAIN_DESCのポインタ
		void SetUpDxgiSwapChainDesc(DXGI_SWAP_CHAIN_DESC* p_Dxgi_);

		// DeviceとSwapChainの作成関数
		// DirectX11のDeviceとSwapChainを作成する
		// まとめた理由は生成関数をD3D11CreateDeviceAndSwapChainにしたため
		bool CreateDeviceAndSwapChain();

		// RenderTargetViewの作成関数
		// DirectX11のRenderTargetViewを作成する
		bool CreateRenderTargetView();

		// DepthバッファStencilバッファ作成関数
		bool CreateDepthAndStencilView();

		bool CreateConstantBuffer();

		bool CreateShader();

		// ViewPort設定関数
		// ContextにViewPortの設定を行う関数
		// ゲーム中に変更がなければ最初に１度行えば問題ない
		void SetUpViewPort();

	private:
		ID3D11Device* p_Device; // DirectX11のDeviceのInterface

		/*
			DeviceContextのInterface
			Contextは２種類あり、描画命令の実行タイミングが変わる
			・ImmediateContext
			　１つだけ作成できるContext
			　このContextで実行した描画命令はすぐにGPUへ発行される
			・DeferredContext
			　このContextで実行した描画命令はコマンドリストへ追加され、
			　コマンドリスト実行命令を使うことで開始される
		*/
		ID3D11DeviceContext* p_Context;
		IDXGISwapChain* p_SwapChain;				// SwapChainのInterface
		ID3D11RenderTargetView* p_RenderTargetView; // RenderTargetViewのInterface
		ID3D11Texture2D* p_DepthStencilTexture;		// ID3D11DepthStencilViewを生成するためのテクスチャ
		ID3D11DepthStencilView* p_DepthStencilView; // DepthStencilViewのInterface
		ID3D11Buffer* p_ConstantBuffer;				// 定数バッファ
		Utility::ConstantBuffer constantBufferData; // ConstantBufferデータ

		shader::Vertex* p_VertexShader;
		shader::Pixcel* p_PixelShader;
		D3D11_VIEWPORT viewPort;

		PolygonData* p_Porigon;
		PolygonData* p_Rect;
		PolygonData* p_Cube;
	};
};



#endif // !DIRECT_GRAPHICS_H
