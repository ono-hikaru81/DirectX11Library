
#include "DirectGraphics.h"

#include <DirectXMath.h>
#include <stdio.h>

#include "../../Utility/Graphics.h"
#include "../Window/Window.h"

#pragma comment(lib, "d3d11.lib")

namespace Engine
{
	// 初期化
	bool DirectGraphics::Initialize()
	{
		if (!CreateDeviceAndSwapChain()) return false;

		if (!CreateRenderTargetView()) return false;

		if (!CreateDepthAndStencilView()) return false;

		if (!CreateShader()) return false;

		SetUpViewPort();

		return true;
	}

	// 解放
	void DirectGraphics::Release()
	{
		p_DepthStencilTexture.Reset();
		p_DepthStencilView.Reset();
		p_Device.Reset();

		if (p_DeviceContext != nullptr)
		{
			p_DeviceContext->ClearState();
			p_DeviceContext->Release();
		}

		p_RenderTargetView.Reset();
		p_SwapChain.Reset();
		p_SamplerState.Reset();

	}

	// 描画開始
	void DirectGraphics::StartRendering()
	{
		// RenderTarget塗りつぶし色
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		// RenderTargetViewのクリア
		p_DeviceContext.Get()->ClearRenderTargetView(p_RenderTargetView.Get(), clearColor);
		// DepthViewとStencilViewのクリア
		p_DeviceContext.Get()->ClearDepthStencilView(p_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	// 描画終了
	void DirectGraphics::FinishRendering()
	{
		// バッファを切り替える
		p_SwapChain.Get()->Present(0, 0);
	}

	void DirectGraphics::SetUpDeviceContext()
	{
		// プリミティブの形状を指定
		p_DeviceContext.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		p_DeviceContext.Get()->VSSetShader(p_ObjFileVertexShader.get()->GetShaderInterface(), nullptr, 0);
		p_DeviceContext.Get()->PSSetShader(p_ObjFilePixelShader.get()->GetShaderInterface(), nullptr, 0);
		p_DeviceContext.Get()->OMSetRenderTargets(1, p_RenderTargetView.GetAddressOf(), p_DepthStencilView.Get());
	}

	// DXGI_SWAP_CHAIN_DESCの設定関数
	void DirectGraphics::SetUpDxgiSwapChainDesc(DXGI_SWAP_CHAIN_DESC* p_Dxgi_)
	{
		HWND windowHandle = FindWindow(Window::p_ClassName.c_str(), nullptr);
		RECT rect;
		GetClientRect(windowHandle, &rect);

		// DirectX11版PresentationParameter
		// バッファの数やサイズ、カラーフォーマット等を設定する
		ZeroMemory(p_Dxgi_, sizeof(DXGI_SWAP_CHAIN_DESC));
		p_Dxgi_->BufferCount = 1;								 // バッファの数
		p_Dxgi_->BufferDesc.Width = (rect.right - rect.left);	 // バッファの横幅
		p_Dxgi_->BufferDesc.Height = (rect.bottom - rect.top);	 // バッファの縦幅
		p_Dxgi_->BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // カラーフォーマット
		p_Dxgi_->BufferDesc.RefreshRate.Numerator = 60;		     // リフレッシュレートの分母
		p_Dxgi_->BufferDesc.RefreshRate.Denominator = 1;		 // リフレッシュシートの分子
		p_Dxgi_->BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // バッファの使い方 Usage => 使用方法
		p_Dxgi_->OutputWindow = windowHandle;					 // 出力対象のウィンドウハンドル
		p_Dxgi_->SampleDesc.Count = 1;							 // マルチサンプリングのサンプル数(未使用は1)
		p_Dxgi_->SampleDesc.Quality = 0;						 // マルチサンプリングの品質(未使用は0)
		p_Dxgi_->Windowed = true;								 // ウィンドウモードの指定
	}

	// DeviceとSwapChainの作成
	bool DirectGraphics::CreateDeviceAndSwapChain()
	{
		// Dxgi => DirectXGraphicsInfrastructure
		DXGI_SWAP_CHAIN_DESC dxgi;
		SetUpDxgiSwapChainDesc(&dxgi);

		D3D_FEATURE_LEVEL level;

		// デバイス生成とスワップチェーン作成を行う
		if (FAILED(D3D11CreateDeviceAndSwapChain(
			nullptr,				  // ビデオアダプタ指定(nullptrは既定)
			D3D_DRIVER_TYPE_HARDWARE, // ドライバのタイプ
			nullptr,				  // D3D_DRIVER_TYPE_SOFTWARE指定時に使用
			0,						  // フラグ指定
			nullptr,				  // D3D_FEATURE_LEVEL指定で自分で定義した配列を指定可能
			0,						  // 上のD3D_FEATURE_LEVEL配列の要素数
			D3D11_SDK_VERSION,		  // SDKバージョン
			&dxgi,					  // DXGI_SWAP_CHAIN_DESC
			p_SwapChain.GetAddressOf(),			  // 関数成功時のSwapChainの出力先
			p_Device.GetAddressOf(),				  // 関数成功時のDeviceの出力先
			&level,					  // 成功したD3D_FEATURE_LEVELの出力先
			p_DeviceContext.GetAddressOf())))		  // 関数成功時のContextの出力先
		{
			return false;
		}

		return true;
	}

	// RenderTargetViewの作成
	bool DirectGraphics::CreateRenderTargetView()
	{
		// RenderTargetViewの対象となるBufferの取得
		ID3D11Texture2D* backBuffer = nullptr;

		if (FAILED(p_SwapChain.Get()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer))) return false;

		// BufferからRendertargetViewの作成
		if (FAILED(p_Device.Get()->CreateRenderTargetView(backBuffer, NULL, p_RenderTargetView.GetAddressOf()))) return false;

		// Targetの取得が終わったので、Bufferを開放
		backBuffer->Release();

		return true;
	}

	// DepthバッファとStencilバッファの作成
	bool DirectGraphics::CreateDepthAndStencilView()
	{
		HWND windowHandle = FindWindow(Window::p_ClassName.c_str(), nullptr);
		RECT rect;
		GetClientRect(windowHandle, &rect);

		// 深度ステンシルバッファ作成
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		textureDesc.Width = (rect.right - rect.left);
		textureDesc.Height = (rect.bottom - rect.top);
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// textureDescの情報でテクスチャを作成
		if (FAILED(p_Device.Get()->CreateTexture2D(&textureDesc, NULL, p_DepthStencilTexture.GetAddressOf()))) return false;

		// Depth、Stencilの情報でテクスチャを作成
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Format = textureDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// CrateTexture2Dとdsv_descからDepthとStencilバッファを作る
		if (FAILED(p_Device.Get()->CreateDepthStencilView(p_DepthStencilTexture.Get(), &depthStencilViewDesc, p_DepthStencilView.GetAddressOf()))) return false;

		return true;
	}

	// シェーダの作成
	bool DirectGraphics::CreateShader()
	{
		p_Porigon2DVertexShader = std::make_unique<Shader::Vertex>();
		if (!p_Porigon2DVertexShader.get()->Create(p_Device.Get(), "Res/Shader/2DPolygonVertexShader.cso")) return false;

		p_Porigon2DPixelShader = std::make_unique<Shader::Pixcel>();
		if (!p_Porigon2DPixelShader.get()->Create(p_Device.Get(), "Res/Shader/2DPolygonPixelShader.cso")) return false;

		p_TextureVertexShader = std::make_unique<Shader::Vertex>();
		if (!p_TextureVertexShader.get()->Create(p_Device.Get(), "Res/Shader/TextureVertexShader.cso")) return false;

		p_TexturePixelShader = std::make_unique<Shader::Pixcel>();
		if (!p_TexturePixelShader.get()->Create(p_Device.Get(), "Res/Shader/TexturePixelShader.cso")) return false;

		p_ObjFileVertexShader = std::make_unique<Shader::Vertex>();
		if (!p_ObjFileVertexShader.get()->Create(p_Device.Get(), "Res/Shader/ObjFileVertexShader.cso")) return false;

		p_ObjFilePixelShader = std::make_unique<Shader::Pixcel>();
		if (!p_ObjFilePixelShader.get()->Create(p_Device.Get(), "Res/Shader/ObjFilePixelShader.cso")) return false;

		return true;
	}

	// ViewPort設定
	void DirectGraphics::SetUpViewPort()
	{
		HWND windowHandle = FindWindow(Window::p_ClassName.c_str(), nullptr);
		RECT rect;
		GetClientRect(windowHandle, &rect);

		// ビューポートの設定
		viewPort =
		{
			.TopLeftX = 0,											// 左上X座標
			.TopLeftY = 0,											// 左上Y座標
			.Width = static_cast<float>(rect.right - rect.left),	// 横幅
			.Height = static_cast<float>(rect.bottom - rect.top),	// 縦幅
			.MinDepth = 0.0f,										// 最小深度
			.MaxDepth = 1.0f,										// 最大深度
		};
		
		// 設定するビューポートの数
		// 設定するビューポート情報のポインタ
		p_DeviceContext.Get()->RSSetViewports(1, &viewPort);
	}
};
