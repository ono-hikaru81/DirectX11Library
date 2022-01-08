
#include "DirectGraphics.h"

#include <stdio.h>

#include "Shader/ShaderBase.h"
#include "../Utility/Graphics.h"
#include "../Window/Window.h"

#pragma comment(lib, "d3d11.lib")

namespace DirectX
{
	bool Graphics::Init()
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

		p_Cube = new PolygonData();
		if (!p_Cube->CreateCube(p_Device, p_VertexShader)) return false;

		return true;
	}

	void Graphics::Release()
	{
		if (p_Context != nullptr)
		{
			p_Context->ClearState();
			p_Context->Release();
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

	void Graphics::StartRendering()
	{
		// RenderTarget塗りつぶし色
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		// DirectX11ではRenderTargetとDepth、Stencilバッファのクリアは別々にする
		// RenderTargetViewのクリア
		p_Context->ClearRenderTargetView(
			p_RenderTargetView, // クリア対象のView
			clearColor);		// クリアカラー

		// DepthViewとStencilViewのクリア
		p_Context->ClearDepthStencilView(
			p_DepthStencilView,						 // クリア対象のView
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, // クリアフラグ(今回はDepth, Stencil共にクリア)
			1.0f,									 // 深度クリア値
			0);										 // ステンシルクリア値
	}

	void Graphics::FinishRendering()
	{
		// バッファを切り替える
		/*
		  第１引数
		  垂直同期のタイミング
			0なら即時描画
			1以上なら指定した値の垂直同期後に描画

		  第２引数
		  出力オプション
			フレーム出力を行う為のオプションで
			基本0で問題ないと思われる
			その他のフラグはDXGI_PRESENTで確認可能
		*/
		p_SwapChain->Present(0, 0);
	}

	void Graphics::DrawPorigon(float posX_, float posY_, float width_, float height_, float angle_)
	{
		UINT strides = sizeof(Utility::Vertex);
		//	DXGI_FORMAT format = DXGI_FORMAT_R16_UINT;
		UINT offsets = 0;
		ID3D11Buffer* vertexBuffer = p_Porigon->GetVertexBuffer();
		ID3D11Buffer* indexBuffer = p_Porigon->GetIndexBuffer();

		p_Context->IASetInputLayout(p_Porigon->GetInputLayout());
		p_Context->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
		p_Context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
		p_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		p_Context->VSSetShader(p_VertexShader->GetShaderInterface(), NULL, 0);
		p_Context->PSSetShader(p_PixelShader->GetShagerInterface(), NULL, 0);
		p_Context->OMSetRenderTargets(1, &p_RenderTargetView, p_DepthStencilView);

		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(angle_ / (180.0f * 3.14f));
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(width_, height_, 1.0f);
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(posX_, posY_, 0);

		worldMatrix = scale * rotateZ * translate;

		XMStoreFloat4x4(&constantBufferData.world, XMMatrixTranspose(worldMatrix));

		constantBufferData.viewPort = DirectX::XMFLOAT4(viewPort.Width, viewPort.Height, 0.0f, 1.0f);

		p_Context->UpdateSubresource(p_ConstantBuffer, 0, NULL, &constantBufferData, 0, 0);

		ID3D11Buffer* p_TempConstantBuffer = p_ConstantBuffer;

		p_Context->VSSetConstantBuffers(0, 1, &p_TempConstantBuffer);

		p_Context->DrawIndexed(3, 0, 0);
	}

	void Graphics::DrawRect(float posX_, float posY_, float width_, float height_, float angle_)
	{
		UINT strides = sizeof(Utility::Vertex);
		//	DXGI_FORMAT format = DXGI_FORMAT_R16_UINT;
		UINT offsets = 0;
		ID3D11Buffer* vertexBuffer = p_Rect->GetVertexBuffer();
		ID3D11Buffer* indexBuffer = p_Rect->GetIndexBuffer();
		// 入力レイアウトの設定
		p_Context->IASetInputLayout(p_Rect->GetInputLayout());
		// 頂点データの設置
		/*
			第1引数 : スロット番号
			第2引数 : 頂点バッファの数
			第3引数 : 頂点バッファの配列
			第4引数 : 1頂点サイズの配列
			第5引数 : オフセットの配列
		*/
		p_Context->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
		// インデックスバッファの設定
		/*
			第1引数 : 設定するインデクスバッファ
			第2引数 : インデックスのサイズフォーマット
			第3引数 : バッファの開始オフセット
		*/
		p_Context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
		// プリミティブの種類。トライアングルリストとかトライアングルストリップとか。
		p_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// 頂点シェーダの設定
		p_Context->VSSetShader(p_VertexShader->GetShaderInterface(), NULL, 0);
		//ピクセルシェーダの設定
		p_Context->PSSetShader(p_PixelShader->GetShagerInterface(), NULL, 0);
		// 出力先の設定
		p_Context->OMSetRenderTargets(1, &p_RenderTargetView, p_DepthStencilView);

		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(angle_ / (180.0f * 3.14f));
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(width_, height_, 1.0f);
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(posX_, posY_, 0);

		worldMatrix = scale * rotateZ * translate;

		XMStoreFloat4x4(&constantBufferData.world, XMMatrixTranspose(worldMatrix));

		constantBufferData.viewPort = DirectX::XMFLOAT4(viewPort.Width, viewPort.Height, 0.0f, 1.0f);

		// コンスタントバッファ更新
		/*
			第1引数 : 更新対象のバッファ
			第2引数 : インデックス(0)
			第3引数 : 更新範囲(NULL)
			第4引数 : 反映データ
			第5引数 : データ1行のサイズ(0)
			第6引数 : 1深度スライスのサイズ(0)
		*/
		p_Context->UpdateSubresource(p_ConstantBuffer, 0, NULL, &constantBufferData, 0, 0);

		ID3D11Buffer* p_TempConstantBuffer = p_ConstantBuffer;

		// コンスタントバッファを設定
		/*
			第1引数 : バッファの開始番号
			第2引数 : バッファの数
			第3引数 : 設定するコンスタントバッファ
		*/
		p_Context->VSSetConstantBuffers(0, 1, &p_TempConstantBuffer);

		// インデックスバッファによる描画
		p_Context->DrawIndexed(6, 0, 0);
	}

	void Graphics::DrawCube(float posX_, float posY_, float width_, float height_, float angle_)
	{
		UINT strides = sizeof(Utility::Vertex);
		//	DXGI_FORMAT format = DXGI_FORMAT_R16_UINT;
		UINT offsets = 0;
		ID3D11Buffer* vertexBuffer = p_Cube->GetVertexBuffer();
		ID3D11Buffer* indexBuffer = p_Cube->GetIndexBuffer();
		p_Context->IASetInputLayout(p_Cube->GetInputLayout());
		p_Context->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
		p_Context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
		p_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		p_Context->VSSetShader(p_VertexShader->GetShaderInterface(), NULL, 0);
		p_Context->PSSetShader(p_PixelShader->GetShagerInterface(), NULL, 0);
		p_Context->OMSetRenderTargets(1, &p_RenderTargetView, p_DepthStencilView);

		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(angle_ / (180.0f * 3.14f));
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(width_, height_, 1.0f);
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(posX_, posY_, 0);

		worldMatrix = scale * rotateZ * translate;

		XMStoreFloat4x4(&constantBufferData.world, XMMatrixTranspose(worldMatrix));

		constantBufferData.viewPort = DirectX::XMFLOAT4(viewPort.Width, viewPort.Height, 0.0f, 1.0f);

		p_Context->UpdateSubresource(p_ConstantBuffer, 0, NULL, &constantBufferData, 0, 0);

		ID3D11Buffer* p_TempConstantBuffer = p_ConstantBuffer;

		p_Context->VSSetConstantBuffers(0, 1, &p_TempConstantBuffer);

		p_Context->DrawIndexed(36, 0, 0);
	}

	void Graphics::SetUpDxgiSwapChainDesc(DXGI_SWAP_CHAIN_DESC* p_Dxgi_)
	{
		HWND windowHandle = FindWindow(Window::p_ClassName, nullptr);
		RECT rect;
		GetClientRect(windowHandle, &rect);

		// DirectX11版PresentationParameter
		//		バッファの数やサイズ、カラーフォーマット等を設定する
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

	bool Graphics::CreateDeviceAndSwapChain()
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
			&p_Context)))			  // 関数成功時のContextの出力先
		{
			return false;
		}

		return true;
	}

	bool Graphics::CreateRenderTargetView()
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

	bool Graphics::CreateDepthAndStencilView()
	{
		HWND windowHandle = FindWindow(Window::p_ClassName, nullptr);
		RECT rect;
		GetClientRect(windowHandle, &rect);

		// 深度ステンシルバッファ作成
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		textureDesc.Width = (rect.right - rect.left);  // 横幅
		textureDesc.Height = (rect.bottom - rect.top); // 縦幅
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		/*
			ミップマップのレベル指定
				どのレベルまで生成するかという設定
				1はマルチサンプリングされたテクスチャを使用するので、
				ミップマップはなしと考えられる
				0は全生成とあるので可能な限りのレベルまでテクスチャが生成されると思われる

			テクスチャ配列のサイズ指定
				テクスチャ配列について調べ切れていないので
				他のサンプルと同様に1を設定しておく

			テクスチャのフォーマット
				DXGI_FORMAT_D24_UNORM_S8_UINTを使用する
				これはおそらくDepth24bit、Stencil8bitとされると思う

				※．UNORMはUnsigned NORMalizedの略で指定された範囲を0.0～1.0にするみたい

			テクスチャフォーマット

			マルチサンプリング設定(使わない)

			テクスチャの使用方法(デフォルト)

			Bind設定はDepth、Stencilに設定

			リソースへのCPUのアクセス権限についての設定
				ReadとWriteがあるが、権限について現状は考える必要はないはずなので、
				デフォルト値であると思われる0をしておく

				※．readとwriteはenum(D3D11_CPU_ACCESS_FLAG)で用意されていた

			texture_descの情報でテクスチャを作成
		*/

		// texture_descの情報でテクスチャを作成
		if (FAILED(p_Device->CreateTexture2D(&textureDesc, NULL, &p_DepthStencilTexture))) return false;

		// Depth、Stencilの情報でテクスチャを作成
		// DepthStencilView
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		ZeroMemory(&dsvDesc, sizeof(dsvDesc));
		dsvDesc.Format = textureDesc.Format;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		/*
			Viewのフォーマット(Textureのモノを使用)

			DSVが何次元であるかとTextureの種類を設定する値
				D3D11_DSV_DIMENSION_TEXTURE2Dのほかにも
				D3D11_DSV_DIMENSION_TEXTURE1Dや
				D3D11_DSV_DIMENSION_TEXTURE2D_ARRAYなどがあったので
				DSVが何次元であるかとTextureの種類を設定するメンバと思われる
				今回は何も通常のTextureとして用意しているはず、
				なので、D3D11_DSV_DIMENSION_TEXTURE2Dを指定

			最初に指定するミップマップのレベルを指定
		*/

		// CrateTexture2Dとdsv_descからDepthとStencilバッファを作る
		if (FAILED(p_Device->CreateDepthStencilView(
			p_DepthStencilTexture, // DSVとして使用されるTexture
			&dsvDesc,			   // DSVの設定
			&p_DepthStencilView))) // ID3D11DepthStencilViewの出力先
		{
			return false;
		}

		return true;
	}

	bool Graphics::CreateConstantBuffer()
	{
		D3D11_BUFFER_DESC constantBufferDesc
		{
			constantBufferDesc.ByteWidth = sizeof(Utility::ConstantBuffer),
			constantBufferDesc.Usage = D3D11_USAGE_DEFAULT,
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
			constantBufferDesc.CPUAccessFlags = 0,
			constantBufferDesc.MiscFlags = 0,
			constantBufferDesc.StructureByteStride = 0
		};

		if (FAILED(p_Device->CreateBuffer(&constantBufferDesc, nullptr, &p_ConstantBuffer)))
		{
			return false;
		}

		return true;
	}

	bool Graphics::CreateShader()
	{
		p_VertexShader = new shader::Vertex();

		if (!p_VertexShader->Create(p_Device, "Res/Shader/VertexShader.cso")) return false;

		p_PixelShader = new shader::Pixcel();

		if (!p_PixelShader->Create(p_Device, "Res/Shader/PixelShader.cso")) return false;

		return true;
	}

	void Graphics::SetUpViewPort()
	{
		HWND windowHandle = FindWindow(Window::p_ClassName, nullptr);
		RECT rect;
		GetClientRect(windowHandle, &rect);

		// ビューポートの設定
		viewPort.TopLeftX = 0; // 左上X座標
		viewPort.TopLeftY = 0; // 左上Y座標
		viewPort.Width = static_cast<float>(rect.right - rect.left);	// 横幅
		viewPort.Height = static_cast<float>(rect.bottom - rect.top); // 縦幅
		viewPort.MinDepth = 0.0f; // 最小深度
		viewPort.MaxDepth = 1.0f; // 最大深度

		// 設定するビューポートの数
		// 設定するビューポート情報のポインタ
		p_Context->RSSetViewports(1, &viewPort);
	}

};
