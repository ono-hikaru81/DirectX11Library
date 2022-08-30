
#include "Polygon.h"

#include "../../Definition.h"
#include "../../Utility/Graphics.h"

namespace Engine
{
	Polygon* Polygon::p_Triangle = nullptr;
	Polygon* Polygon::p_Rect = nullptr;

	// 初期化
	bool Polygon::Initialize(DirectGraphics* p_DirectGraphics_)
	{
		p_Triangle = new Polygon();
		p_Rect = new Polygon();

		if (!CreateConstantBuffer(p_DirectGraphics_)) return false;

		if (!p_Triangle->CreateTriangleData(p_DirectGraphics_->GetDevice(), p_DirectGraphics_->GetPorigon2DVertexShader())) return false;

		if (!p_Rect->CreateRectData(p_DirectGraphics_->GetDevice(), p_DirectGraphics_->GetPorigon2DVertexShader())) return false;

		return true;
	}

	// 解放
	void Polygon::Release()
	{
		p_VertexBuffer.Reset();
		p_IndexBuffer.Reset();
		p_InputLayout.Reset();
		p_ConstantBuffer.Reset();
	}

	// 三角形描画
	void Polygon::RenderTriangle(DirectGraphics* p_DirectGraphics_, const float& posX_, const float& posY_, const float& width_, const float& height_, const float angle_, const Utility::Vector color_)
	{
		UINT strides = sizeof(Utility::Porygon2D::Vertex);
		UINT offsets = 0;
		ID3D11DeviceContext* p_DeviceContext = p_DirectGraphics_->GetDeviceContext();

		// 入力レイアウトの設定
		p_DeviceContext->IASetInputLayout(p_Triangle->p_InputLayout.Get());
		// 頂点データの設定
		p_DeviceContext->IASetVertexBuffers(0, 1, p_Triangle->p_VertexBuffer.GetAddressOf(), &strides, &offsets);
		// インデックスバッファの設定
		p_DeviceContext->IASetIndexBuffer(p_Triangle->p_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		// プリミティブ型の設定
		p_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// 頂点シェーダの設定
		p_DeviceContext->VSSetShader(p_DirectGraphics_->GetPorigon2DVertexShader()->GetShaderInterface(), nullptr, 0);
		// ピクセルシェーダの設定
		p_DeviceContext->PSSetShader(p_DirectGraphics_->GetPorigon2DPixelShader()->GetShaderInterface(), nullptr, 0);
		// 出力先の設定
		p_DeviceContext->OMSetRenderTargets(1, p_DirectGraphics_->GetRenderTargetViewAddress(), p_DirectGraphics_->GetDepthStencilView());

		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(angle_ * (3.14f / 180.0f));
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(width_, height_, 1.0f);
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(posX_, posY_, 0);

		worldMatrix = scale * rotateZ * translate;

		XMStoreFloat4x4(&constantBufferData.world, XMMatrixTranspose(worldMatrix));

		constantBufferData.viewPort = DirectX::XMFLOAT4(p_DirectGraphics_->GetViewPort()->Width, p_DirectGraphics_->GetViewPort()->Height, 0.0f, 1.0f);
		constantBufferData.color = DirectX::XMFLOAT4(color_.GetX(), color_.GetY(), color_.GetZ(), 1.0f);

		// コンスタントバッファ更新
		p_DeviceContext->UpdateSubresource(p_ConstantBuffer.Get(), 0, nullptr, &constantBufferData, 0, 0);
		// コンスタントバッファを設定
		p_DeviceContext->VSSetConstantBuffers(0, 1, p_ConstantBuffer.GetAddressOf());
		// インデックスバッファによる描画
		p_DeviceContext->DrawIndexed(3, 0, 0);
	}

	// 矩形描画
	void Polygon::RenderRect(DirectGraphics* p_DirectGraphics_, const float& posX_, const float& posY_, const float& width_, const float& height_, const float angle_, const Utility::Vector color_)
	{
		UINT strides = sizeof(Utility::Porygon2D::Vertex);
		UINT offsets = 0;
		ID3D11DeviceContext* p_DeviceContext = p_DirectGraphics_->GetDeviceContext();

		// 入力レイアウトの設定
		p_DeviceContext->IASetInputLayout(p_Rect->p_InputLayout.Get());
		// 頂点データの設置
		p_DeviceContext->IASetVertexBuffers(0, 1, p_Rect->p_VertexBuffer.GetAddressOf(), &strides, &offsets);
		// インデックスバッファの設定
		p_DeviceContext->IASetIndexBuffer(p_Rect->p_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		// プリミティブ型の設定
		p_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		// 頂点シェーダの設定
		p_DeviceContext->VSSetShader(p_DirectGraphics_->GetPorigon2DVertexShader()->GetShaderInterface(), nullptr, 0);
		// ピクセルシェーダの設定
		p_DeviceContext->PSSetShader(p_DirectGraphics_->GetPorigon2DPixelShader()->GetShaderInterface(), nullptr, 0);
		// 出力先の設定
		p_DeviceContext->OMSetRenderTargets(1, p_DirectGraphics_->GetRenderTargetViewAddress(), p_DirectGraphics_->GetDepthStencilView());

		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(angle_ * (3.14f / 180.0f));
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(width_, height_, 1.0f);
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(posX_, posY_, 0);

		worldMatrix = scale * rotateZ * translate;

		XMStoreFloat4x4(&constantBufferData.world, XMMatrixTranspose(worldMatrix));

		constantBufferData.viewPort = DirectX::XMFLOAT4(p_DirectGraphics_->GetViewPort()->Width, p_DirectGraphics_->GetViewPort()->Height, 0.0f, 1.0f);
		constantBufferData.color = DirectX::XMFLOAT4(color_.GetX(), color_.GetY(), color_.GetZ(), 1.0f);

		// コンスタントバッファ更新
		p_DeviceContext->UpdateSubresource(p_ConstantBuffer.Get(), 0, nullptr, &constantBufferData, 0, 0);
		// コンスタントバッファを設定
		p_DeviceContext->VSSetConstantBuffers(0, 1, p_ConstantBuffer.GetAddressOf());
		// インデックスバッファによる描画
		p_DeviceContext->DrawIndexed(4, 0, 0);
	}

	// 三角形作成
	bool Polygon::CreateTriangleData(ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_)
	{
		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC vertexDesc[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// 頂点データ作成
		Utility::Porygon2D::Vertex vertexList[] =
		{
			{ { -0.5f,  0.5f, 0.0f, 1.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ {  0.5f, -0.5f, 0.0f, 1.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.5f, -0.5f, 0.0f, 1.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } }
		};

		// 頂点番号
		UWORD vertexNumber[]
		{ 
			0, 1, 2 
		};

		// 頂点バッファ作成
		D3D11_BUFFER_DESC vertexBufferDesc
		{
			.ByteWidth = sizeof(Utility::Porygon2D::Vertex) * 3,
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_VERTEX_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = 0,
			.StructureByteStride = sizeof(Utility::Porygon2D::Vertex)
		};

		// 頂点バッファの初期データ
		D3D11_SUBRESOURCE_DATA vertexInitData
		{
			.pSysMem = vertexList,
			.SysMemPitch = 0,
			.SysMemSlicePitch = 0
		};

		// バッファ作成
		if (FAILED(p_Device_->CreateBuffer(&vertexBufferDesc, &vertexInitData, p_VertexBuffer.GetAddressOf()))) return false;

		// インデックスバッファ作成
		D3D11_BUFFER_DESC indexBufferDesc
		{
			.ByteWidth = sizeof(UWORD) * 3,
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_INDEX_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = 0,
			.StructureByteStride = 0
		};

		// インデックスバッファ作成
		D3D11_SUBRESOURCE_DATA indexInitData
		{
			.pSysMem = vertexNumber,
			.SysMemPitch = 0,
			.SysMemSlicePitch = 0
		};

		// バッファ作成
		if (FAILED(p_Device_->CreateBuffer(&indexBufferDesc, &indexInitData, p_IndexBuffer.GetAddressOf()))) return false;

		// 入力レイアウト作成
		if (FAILED(p_Device_->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), p_VertexShader_->GetData(), p_VertexShader_->GetSize(), p_InputLayout.GetAddressOf()))) return false;

		return true;
	}

	// 矩形作成
	bool Polygon::CreateRectData(ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_)
	{
		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC vertexDesc[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// 頂点データ作成
		Utility::Porygon2D::Vertex vertexList[]
		{
			{ { -0.5f,  0.5f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ {  0.5f,  0.5f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.5f, -0.5f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
			{ {  0.5f, -0.5f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } }

		};

		// 頂点番号
		UWORD vertexNumber[]
		{
			0, 1, 2, 3,
		};

		// 頂点バッファ作成
		D3D11_BUFFER_DESC vertexBufferDesc
		{
			.ByteWidth = sizeof(Utility::Porygon2D::Vertex) * 4,
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_VERTEX_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = 0,
			.StructureByteStride = sizeof(Utility::Porygon2D::Vertex)
		};

		// 頂点バッファの初期データ
		D3D11_SUBRESOURCE_DATA vertexInitDesc
		{
			.pSysMem = vertexList,
			.SysMemPitch = 0,
			.SysMemSlicePitch = 0
		};

		// バッファ作成
		if (FAILED(p_Device_->CreateBuffer(&vertexBufferDesc, &vertexInitDesc, p_VertexBuffer.GetAddressOf()))) return false;

		// インデックスバッファ作成
		D3D11_BUFFER_DESC indexBufferDesc
		{
			.ByteWidth = sizeof(UWORD) * 6,
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_INDEX_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = 0,
			.StructureByteStride = 0
		};

		// インデックスバッファ作成
		D3D11_SUBRESOURCE_DATA indexInitData
		{
			.pSysMem = vertexNumber,
			.SysMemPitch = 0,
			.SysMemSlicePitch = 0
		};

		// バッファ作成
		if (FAILED(p_Device_->CreateBuffer(&indexBufferDesc, &indexInitData, p_IndexBuffer.GetAddressOf()))) return false;

		// 頂点レイアウト作成
		if (FAILED(p_Device_->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), p_VertexShader_->GetData(), p_VertexShader_->GetSize(), p_InputLayout.GetAddressOf()))) return false;

		return true;
	}

	// 定数バッファの作成
	bool Polygon::CreateConstantBuffer(DirectGraphics* p_DirectGraphics_)
	{
		D3D11_BUFFER_DESC constantBufferDesc
		{
			.ByteWidth = sizeof(Utility::Porygon2D::ConstantBuffer),
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = 0,
			.StructureByteStride = 0
		};

		if (FAILED(p_DirectGraphics_->GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, p_ConstantBuffer.GetAddressOf()))) return false;

		return true;
	}
};
