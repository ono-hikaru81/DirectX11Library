
#include "DirectGraphics.h"

#include <DirectXMath.h>
#include <stdio.h>

#include "../../Utility/Graphics.h"
#include "../Window/Window.h"
#include "../Texture/WICTextureLoader.h"

#pragma comment(lib, "d3d11.lib")

namespace Engine
{
	// 初期化
	bool DirectGraphics::Initialize()
	{
		if (!CreateDeviceAndSwapChain()) return false;

		if (!CreateRenderTargetView()) return false;

		if (!CreateDepthAndStencilView()) return false;

		if (!CreateConstantBuffer()) return false;

		if (!CreateObjFileConstantBuffer()) return false;

		if (!CreateShader()) return false;

		SetUpViewPort();

		// 変換行列設定
		SetUpTransform();

		p_Porigon = std::make_unique<PolygonData>();
		if (!p_Porigon.get()->CreatePorigon(p_Device.Get(), p_2DPorigonVertexShader.get())) return false;

		p_Rect = std::make_unique<PolygonData>();
		if (!p_Rect.get()->CreateRect(p_Device.Get(), p_2DPorigonVertexShader.get())) return false;

		p_Texture = std::make_unique<Texture>();
		if (!p_Texture.get()->CreatePorigonModel(p_Device.Get(), p_TextureVertexShader.get())) return false;

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

	void DirectGraphics::SetUpTransform()
	{
		HWND windowHandle = FindWindow(Window::p_ClassName.c_str(), nullptr);
		RECT rect{};
		GetClientRect(windowHandle, &rect);

		// Viewマトリクス設定
		DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.0f, -50.0f, 0.0f);
		DirectX::XMVECTOR focus = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(eye, focus, up);

		// プロジェクションマトリクス設定
		constexpr float fov = DirectX::XMConvertToRadians(45.0f);
		float aspect = static_cast<float>(rect.right - rect.left) / (rect.bottom - rect.top);
		float nearZ = 0.1f;
		float farZ = 100.0f;
		DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);

		// ライトの設定
		DirectX::XMVECTOR light = DirectX::XMVector3Normalize(DirectX::XMVectorSet(0.0f, 0.5f, -1.0f, 0.0f));

		// コンスタントバッファの設定
		XMStoreFloat4x4(&objFileConstantBufferData.view, XMMatrixTranspose(viewMatrix));
		XMStoreFloat4x4(&objFileConstantBufferData.projection, XMMatrixTranspose(projectionMatrix));
		XMStoreFloat4(&objFileConstantBufferData.lightVector, light);

		// ライトのカラー設定
		objFileConstantBufferData.lightColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void DirectGraphics::SetUpDeviceContext()
	{
		// プリミティブの形状を指定
		p_DeviceContext.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		p_DeviceContext.Get()->VSSetShader(p_ObjFileVertexShader.get()->GetShaderInterface(), nullptr, 0);
		p_DeviceContext.Get()->PSSetShader(p_ObjFilePixelShader.get()->GetShaderInterface(), nullptr, 0);
		p_DeviceContext.Get()->OMSetRenderTargets(1, p_RenderTargetView.GetAddressOf(), p_DepthStencilView.Get());
	}

	// ポリゴン描画
	void DirectGraphics::DrawPorigon(float posX_, float posY_, float width_, float height_, float angle_)
	{
		UINT strides = sizeof(Utility::Porigon2D::Vertex);
		UINT offsets = 0;
		ID3D11Buffer* p_VertexBuffer = p_Porigon.get()->GetVertexBuffer();
		ID3D11Buffer* p_IndexBuffer = p_Porigon.get()->GetIndexBuffer();

		// 入力レイアウトの設定
		p_DeviceContext.Get()->IASetInputLayout(p_Porigon.get()->GetInputLayout());
		// 頂点データの.Get()設置
		p_DeviceContext.Get()->IASetVertexBuffers(0, 1, &p_VertexBuffer, &strides, &offsets);
		// インデックス.Get()バッファの設定
		p_DeviceContext.Get()->IASetIndexBuffer(p_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		// プリミティブ.Get()型の設定
		p_DeviceContext.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// 頂点シェーダ.Get()の設定
		p_DeviceContext.Get()->VSSetShader(p_2DPorigonVertexShader.get()->GetShaderInterface(), NULL, 0);
		// ピクセルシェ.Get()ーダの設定
		p_DeviceContext.Get()->PSSetShader(p_2DPorigonPixelShader.get()->GetShaderInterface(), NULL, 0);
		// 出力先の設定.Get()
		p_DeviceContext.Get()->OMSetRenderTargets(1, p_RenderTargetView.GetAddressOf(), p_DepthStencilView.Get());

		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(angle_ / (180.0f * 3.14f));
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(width_, height_, 1.0f);
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(posX_, posY_, 0);

		worldMatrix = scale * rotateZ * translate;

		XMStoreFloat4x4(&constantBufferData.world, XMMatrixTranspose(worldMatrix));

		constantBufferData.viewPort = DirectX::XMFLOAT4(viewPort.Width, viewPort.Height, 0.0f, 1.0f);

		// コンスタントバッファ更新
		p_DeviceContext.Get()->UpdateSubresource(p_ConstantBuffer.Get(), 0, NULL, &constantBufferData, 0, 0);

		// コンスタントバッファを設定
		p_DeviceContext.Get()->VSSetConstantBuffers(0, 1, p_ConstantBuffer.GetAddressOf());
		// インデックスバッファによる描画
		p_DeviceContext.Get()->DrawIndexed(3, 0, 0);
	}

	// 矩形描画
	void DirectGraphics::DrawRect(float posX_, float posY_, float width_, float height_, float angle_)
	{
		UINT strides = sizeof(Utility::Porigon2D::Vertex);
		UINT offsets = 0;
		ID3D11Buffer* p_VertexBuffer = p_Rect.get()->GetVertexBuffer();
		ID3D11Buffer* p_IndexBuffer = p_Rect.get()->GetIndexBuffer();

		// 入力レイアウトの設定
		p_DeviceContext.Get()->IASetInputLayout(p_Rect.get()->GetInputLayout());
		// 頂点データの設置
		p_DeviceContext.Get()->IASetVertexBuffers(0, 1, &p_VertexBuffer, &strides, &offsets);
		// インデックスバッファの設定
		p_DeviceContext.Get()->IASetIndexBuffer(p_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		// プリミティブ型の設定
		p_DeviceContext.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		// 頂点シェーダの設定
		p_DeviceContext.Get()->VSSetShader(p_2DPorigonVertexShader.get()->GetShaderInterface(), NULL, 0);
		// ピクセルシェーダの設定
		p_DeviceContext.Get()->PSSetShader(p_2DPorigonPixelShader.get()->GetShaderInterface(), NULL, 0);
		// 出力先の設定
		p_DeviceContext.Get()->OMSetRenderTargets(1, p_RenderTargetView.GetAddressOf(), p_DepthStencilView.Get());

		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(angle_ / (180.0f * 3.14f));
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(width_, height_, 1.0f);
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(posX_, posY_, 0);

		worldMatrix = scale * rotateZ * translate;

		XMStoreFloat4x4(&constantBufferData.world, XMMatrixTranspose(worldMatrix));

		constantBufferData.viewPort = DirectX::XMFLOAT4(viewPort.Width, viewPort.Height, 0.0f, 1.0f);

		// コンスタントバッファ更新
		p_DeviceContext.Get()->UpdateSubresource(p_ConstantBuffer.Get(), 0, NULL, &constantBufferData, 0, 0);

		// コンスタントバッファを設定
		p_DeviceContext.Get()->VSSetConstantBuffers(0, 1, p_ConstantBuffer.GetAddressOf());
		// インデックスバッファによる描画
		p_DeviceContext.Get()->DrawIndexed(4, 0, 0);
	}

	// テクスチャ読み込み
	bool DirectGraphics::LoadTexture(const std::wstring fileName_)
	{
		if (textureList.count(fileName_) > 0 && textureList[fileName_] != nullptr) return false;

		if (FAILED(DirectX::CreateWICTextureFromFile(p_Device.Get(), fileName_.c_str(), nullptr, &textureList[fileName_]))) return false;

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
		ID3D11Buffer* buffer = p_Texture.get()->GetVertexBuffer();
		ID3D11Buffer* p_IndexBuffer = p_Texture.get()->GetIndexBuffer();

		// 入力レイアウトの設定
		p_DeviceContext.Get()->IASetInputLayout(p_Texture.get()->GetInputLayput());
		// 頂点シェーダの設定
		p_DeviceContext.Get()->VSSetShader(p_TextureVertexShader.get()->GetShaderInterface(), nullptr, 0);
		// インデックスバッファの設定
		p_DeviceContext.Get()->IASetIndexBuffer(p_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		// プリミティブ型の設定
		p_DeviceContext.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		// 頂点データの設置
		p_DeviceContext.Get()->IASetVertexBuffers(0, 1, &buffer, &strides, &offsets);
		// ピクセルシェーダのサンプラを設定
		p_DeviceContext.Get()->PSSetSamplers(0, 1, p_SamplerState.GetAddressOf());
		// ピクセルシェーダで使用するテクスチャを設定
		p_DeviceContext.Get()->PSSetShaderResources(0, 1, &textureList[fileName_]);
		// ピクセルシェーダの設定
		p_DeviceContext.Get()->PSSetShader(p_TexturePixelShader.get()->GetShaderInterface(), nullptr, 0);
		// 出力先の設定
		p_DeviceContext.Get()->OMSetRenderTargets(1, p_RenderTargetView.GetAddressOf(), p_DepthStencilView.Get());

		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(angle_ / (180.0f * 3.14f));
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(width_, height_, 1.0f);
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(posX_, posY_, 0);

		worldMatrix = scale * rotateZ * translate;

		XMStoreFloat4x4(&constantBufferData.world, XMMatrixTranspose(worldMatrix));

		constantBufferData.viewPort = DirectX::XMFLOAT4(viewPort.Width, viewPort.Height, 0.0f, 1.0f);

		// コンスタントバッファ更新
		p_DeviceContext.Get()->UpdateSubresource(p_ConstantBuffer.Get(), 0, NULL, &constantBufferData, 0, 0);

		// コンスタントバッファを設定
		p_DeviceContext.Get()->VSSetConstantBuffers(0, 1, p_ConstantBuffer.GetAddressOf());

		// 描画
		p_DeviceContext.Get()->DrawIndexed(4, 0, 0);
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

	// ConstantBufferの作成
	bool DirectGraphics::CreateConstantBuffer()
	{
		D3D11_BUFFER_DESC constantBufferDesc
		{
			.ByteWidth = sizeof(Utility::Porigon2D::ConstantBuffer),
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = 0,
			.StructureByteStride = 0
		};

		if (FAILED(p_Device.Get()->CreateBuffer(&constantBufferDesc, nullptr, p_ConstantBuffer.GetAddressOf()))) return false;

		return true;
	}

	bool DirectGraphics::CreateObjFileConstantBuffer()
	{
		D3D11_BUFFER_DESC constantBufferDesc
		{
			.ByteWidth = sizeof(Utility::ObjFile::ConstantBuffer),
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = 0,
			.StructureByteStride = sizeof(Utility::ObjFile::ConstantBuffer)
		};

		if (FAILED(p_Device.Get()->CreateBuffer(&constantBufferDesc, nullptr, p_ObjFileConstantBuffer.GetAddressOf()))) return false;

		return true;
	}

	// シェーダの作成
	bool DirectGraphics::CreateShader()
	{
		p_2DPorigonVertexShader = std::make_unique<Shader::Vertex>();
		if (!p_2DPorigonVertexShader.get()->Create(p_Device.Get(), "Res/Shader/2DPolygonVertexShader.cso")) return false;

		p_2DPorigonPixelShader = std::make_unique<Shader::Pixcel>();
		if (!p_2DPorigonPixelShader.get()->Create(p_Device.Get(), "Res/Shader/2DPolygonPixelShader.cso")) return false;

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
