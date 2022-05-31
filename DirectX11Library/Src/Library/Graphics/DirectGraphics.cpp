
#include "DirectGraphics.h"

#include <stdio.h>

#include "../Shader/ShaderBase.h"
#include "../../Utility/Graphics.h"
#include "../Window/Window.h"
#include "../Texture/WICTextureLoader.h"

#pragma comment(lib, "d3d11.lib")

namespace Library
{
	// 初期化
	bool DirectGraphics::Initialize()
	{
		if (!CreateDeviceAndSwapChain()) return false;

		if (!CreateRenderTargetView()) return false;

		if (!CreateDepthAndStencilView()) return false;

		if (!CreateConstantBuffer()) return false;

		SetUpViewPort();

		if (!CreateShader()) return false;

		p_Porigon = new PolygonData();
		if (!p_Porigon->CreatePorigon(p_Device, p_VertexShader)) return false;

		p_Rect = new PolygonData();
		if (!p_Rect->CreateRect(p_Device, p_VertexShader)) return false;

		p_Texture = new Texture();
		if (!p_Texture->CreatePorigonModel(p_Device, p_TextureVertexShader)) return false;

		return true;
	}

	// 解放
	void DirectGraphics::Release()
	{
		if (p_DeviceContext != nullptr)
		{
			p_DeviceContext->ClearState();
			p_DeviceContext->Release();
		}

		if (p_SwapChain != nullptr)
		{
			p_SwapChain->Release();
		}

		if (p_Device != nullptr)
		{
			p_Device->Release();
		}
	}

	// 描画開始
	void DirectGraphics::StartRendering()
	{
		// RenderTarget塗りつぶし色
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		// RenderTargetViewのクリア
		p_DeviceContext->ClearRenderTargetView(p_RenderTargetView, clearColor);
		// DepthViewとStencilViewのクリア
		p_DeviceContext->ClearDepthStencilView(p_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	// 描画終了
	void DirectGraphics::FinishRendering()
	{
		// バッファを切り替える
		p_SwapChain->Present(0, 0);
	}

	// ポリゴン描画
	void DirectGraphics::DrawPorigon(float posX_, float posY_, float width_, float height_, float angle_)
	{
		UINT strides = sizeof(Utility::Vertex);
		UINT offsets = 0;
		ID3D11Buffer* p_VertexBuffer = p_Porigon->GetVertexBuffer();
		ID3D11Buffer* p_IndexBuffer = p_Porigon->GetIndexBuffer();

		// 入力レイアウトの設定
		p_DeviceContext->IASetInputLayout(p_Porigon->GetInputLayout());
		// 頂点データの設置
		p_DeviceContext->IASetVertexBuffers(0, 1, &p_VertexBuffer, &strides, &offsets);
		// インデックスバッファの設定
		p_DeviceContext->IASetIndexBuffer(p_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		// プリミティブ型の設定
		p_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// 頂点シェーダの設定
		p_DeviceContext->VSSetShader(p_VertexShader->GetShaderInterface(), NULL, 0);
		// ピクセルシェーダの設定
		p_DeviceContext->PSSetShader(p_PixelShader->GetShaderInterface(), NULL, 0);
		// 出力先の設定
		p_DeviceContext->OMSetRenderTargets(1, &p_RenderTargetView, p_DepthStencilView);

		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(angle_ / (180.0f * 3.14f));
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(width_, height_, 1.0f);
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(posX_, posY_, 0);

		worldMatrix = scale * rotateZ * translate;

		XMStoreFloat4x4(&constantBufferData.world, XMMatrixTranspose(worldMatrix));

		constantBufferData.viewport = DirectX::XMFLOAT4(viewport.Width, viewport.Height, 0.0f, 1.0f);

		// コンスタントバッファ更新
		p_DeviceContext->UpdateSubresource(p_ConstantBuffer, 0, NULL, &constantBufferData, 0, 0);

		ID3D11Buffer* p_TempConstantBuffer = p_ConstantBuffer;

		// コンスタントバッファを設定
		p_DeviceContext->VSSetConstantBuffers(0, 1, &p_TempConstantBuffer);
		// インデックスバッファによる描画
		p_DeviceContext->DrawIndexed(3, 0, 0);
	}

	// 矩形描画
	void DirectGraphics::DrawRect(float posX_, float posY_, float width_, float height_, float angle_)
	{
		UINT strides = sizeof(Utility::Vertex);
		UINT offsets = 0;
		ID3D11Buffer* p_VertexBuffer = p_Rect->GetVertexBuffer();
		ID3D11Buffer* p_IndexBuffer = p_Rect->GetIndexBuffer();

		// 入力レイアウトの設定
		p_DeviceContext->IASetInputLayout(p_Rect->GetInputLayout());
		// 頂点データの設置
		p_DeviceContext->IASetVertexBuffers(0, 1, &p_VertexBuffer, &strides, &offsets);
		// インデックスバッファの設定
		p_DeviceContext->IASetIndexBuffer(p_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		// プリミティブ型の設定
		p_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// 頂点シェーダの設定
		p_DeviceContext->VSSetShader(p_VertexShader->GetShaderInterface(), NULL, 0);
		// ピクセルシェーダの設定
		p_DeviceContext->PSSetShader(p_PixelShader->GetShaderInterface(), NULL, 0);
		// 出力先の設定
		p_DeviceContext->OMSetRenderTargets(1, &p_RenderTargetView, p_DepthStencilView);

		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(angle_ / (180.0f * 3.14f));
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(width_, height_, 1.0f);
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(posX_, posY_, 0);

		worldMatrix = scale * rotateZ * translate;

		XMStoreFloat4x4(&constantBufferData.world, XMMatrixTranspose(worldMatrix));

		constantBufferData.viewport = DirectX::XMFLOAT4(viewport.Width, viewport.Height, 0.0f, 1.0f);

		// コンスタントバッファ更新
		p_DeviceContext->UpdateSubresource(p_ConstantBuffer, 0, NULL, &constantBufferData, 0, 0);

		ID3D11Buffer* p_TempConstantBuffer = p_ConstantBuffer;

		// コンスタントバッファを設定
		p_DeviceContext->VSSetConstantBuffers(0, 1, &p_TempConstantBuffer);
		// インデックスバッファによる描画
		p_DeviceContext->DrawIndexed(6, 0, 0);
	}

	// テクスチャ読み込み
	bool DirectGraphics::Loadtexture(const std::wstring fileName_)
	{
		if (textureList.count(fileName_) > 0 && textureList[fileName_] != nullptr) return false;

		if (FAILED(DirectX::CreateWICTextureFromFile(p_Device, fileName_.c_str(), nullptr, &textureList[fileName_]))) return false;

		return true;
	}

	// テクスチャ解放
	void DirectGraphics::ReleaseTexture(const std::wstring fileName_)
	{
		if (textureList.count(fileName_) != 0 && textureList[fileName_] != nullptr)
		{
			textureList[fileName_]->Release();
			textureList.erase(fileName_);
		}
	}

	// 全てのテクスチャを解放
	void DirectGraphics::ReleaseAllTexture()
	{
		for (auto& texture : textureList)
		{
			if (texture.second == nullptr) continue;

			texture.second->Release();
		}

		textureList.clear();
	}

	// テクスチャ描画
	void DirectGraphics::DrawTexture(const std::wstring fileName_, float posX_, float posY_, float width_, float height_, float angle_ )
	{
		UINT strides = sizeof(Utility::TextureVertex);
		UINT offsets = 0;
		ID3D11Buffer* buffer = p_Texture->GetVertexBuffer();
		ID3D11Buffer* p_IndexBuffer = p_Texture->GetIndexBuffer();

		// 入力レイアウトの設定
		p_DeviceContext->IASetInputLayout(p_Texture->GetInputLayput());
		// 頂点シェーダの設定
		p_DeviceContext->VSSetShader(p_TextureVertexShader->GetShaderInterface(), nullptr, 0);
		// インデックスバッファの設定
		p_DeviceContext->IASetIndexBuffer(p_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		// プリミティブ型の設定
		p_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		// 頂点データの設置
		p_DeviceContext->IASetVertexBuffers(0, 1, &buffer, &strides, &offsets);
		// ピクセルシェーダのサンプラを設定
		p_DeviceContext->PSSetSamplers(0, 1, &p_SamplerState);
		// ピクセルシェーダで使用するテクスチャを設定
		p_DeviceContext->PSSetShaderResources(0, 1, &textureList[fileName_]);
		// ピクセルシェーダの設定
		p_DeviceContext->PSSetShader(p_TexturePixelShader->GetShaderInterface(), nullptr, 0);
		// 出力先の設定
		p_DeviceContext->OMSetRenderTargets(1, &p_RenderTargetView, p_DepthStencilView);

		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(angle_ / (180.0f * 3.14f));
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(width_, height_, 1.0f);
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(posX_, posY_, 0);

		worldMatrix = scale * rotateZ * translate;

		XMStoreFloat4x4(&constantBufferData.world, XMMatrixTranspose(worldMatrix));

		constantBufferData.viewport = DirectX::XMFLOAT4(viewport.Width, viewport.Height, 0.0f, 1.0f);

		// コンスタントバッファ更新
		p_DeviceContext->UpdateSubresource(p_ConstantBuffer, 0, NULL, &constantBufferData, 0, 0);

		ID3D11Buffer* p_TempConstantBuffer = p_ConstantBuffer;

		// コンスタントバッファを設定
		p_DeviceContext->VSSetConstantBuffers(0, 1, &p_TempConstantBuffer);

		// 描画
		p_DeviceContext->DrawIndexed(6, 0, 0);
	}

	void DirectGraphics::SetUpDxgiSwapChainDesc(DXGI_SWAP_CHAIN_DESC* p_Dxgi_)
	{
		HWND windowHandle = FindWindow(Window::p_ClassName, nullptr);
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
			&p_SwapChain,			  // 関数成功時のSwapChainの出力先
			&p_Device,				  // 関数成功時のDeviceの出力先
			&level,					  // 成功したD3D_FEATURE_LEVELの出力先
			&p_DeviceContext)))		  // 関数成功時のContextの出力先
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

		if (FAILED(p_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer))) return false;

		// BufferからRendertargetViewの作成
		if (FAILED(p_Device->CreateRenderTargetView(backBuffer, NULL, &p_RenderTargetView))) return false;

		// Targetの取得が終わったので、Bufferを開放
		backBuffer->Release();

		return true;
	}

	// DepthバッファとStencilバッファの作成
	bool DirectGraphics::CreateDepthAndStencilView()
	{
		HWND windowHandle = FindWindow(Window::p_ClassName, nullptr);
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
		if (FAILED(p_Device->CreateTexture2D(&textureDesc, NULL, &p_DepthStencilTexture))) return false;

		// Depth、Stencilの情報でテクスチャを作成
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Format = textureDesc.Format;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// CrateTexture2Dとdsv_descからDepthとStencilバッファを作る
		if (FAILED(p_Device->CreateDepthStencilView(p_DepthStencilTexture, &depthStencilViewDesc, &p_DepthStencilView))) return false;

		return true;
	}

	// ConstantBufferの作成
	bool DirectGraphics::CreateConstantBuffer()
	{
		D3D11_BUFFER_DESC constantBufferDesc
		{
			.ByteWidth = sizeof(Utility::ConstantBuffer),
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = 0,
			.StructureByteStride = 0
		};

		if (FAILED(p_Device->CreateBuffer(&constantBufferDesc, nullptr, &p_ConstantBuffer))) return false;

		return true;
	}

	// シェーダの作成
	bool DirectGraphics::CreateShader()
	{
		p_VertexShader = new Shader::Vertex();
		if (!p_VertexShader->Create(p_Device, "Res/Shader/2DPolygonVertexShader.cso")) return false;

		p_PixelShader = new Shader::Pixcel();
		if (!p_PixelShader->Create(p_Device, "Res/Shader/2DPolygonPixelShader.cso")) return false;

		p_TextureVertexShader = new Shader::Vertex();
		if (!p_TextureVertexShader->Create(p_Device, "Res/Shader/TextureVertexShader.cso")) return false;

		p_TexturePixelShader = new Shader::Pixcel();
		if (!p_TexturePixelShader->Create(p_Device, "Res/Shader/TexturePixelShader.cso")) return false;

		return true;
	}

	// ViewPort設定
	void DirectGraphics::SetUpViewPort()
	{
		HWND windowHandle = FindWindow(Window::p_ClassName, nullptr);
		RECT rect;
		GetClientRect(windowHandle, &rect);

		// ビューポートの設定
		viewport =
		{
			.TopLeftX = 0,	// 左上X座標
			.TopLeftY = 0,	// 左上Y座標
			.Width = static_cast<float>(rect.right - rect.left),	// 横幅
			.Height = static_cast<float>(rect.bottom - rect.top),	// 縦幅
			.MinDepth = 0.0f,	// 最小深度
			.MaxDepth = 1.0f,	// 最大深度
		};
		
		// 設定するビューポートの数
		// 設定するビューポート情報のポインタ
		p_DeviceContext->RSSetViewports(1, &viewport);
	}
};
