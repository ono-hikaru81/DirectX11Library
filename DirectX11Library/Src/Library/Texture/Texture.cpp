
#include "Texture.h"


#include "../Texture/WICTextureLoader.h"
#include "../../Utility/Graphics.h"


namespace Engine
{
	// 初期化
	bool Texture::Initialize(DirectGraphics* p_DirectGraphics_)
	{
		if (!CreateConstantBuffer(p_DirectGraphics_)) return false;

		if (!CreatePolygonModel(p_DirectGraphics_->GetDevice(), p_DirectGraphics_->GetTextureVertexShader())) return false;

		return true;
	}

	// 解放
	void Texture::Release()
	{
		p_VertexBuffer.Reset();
		p_IndexBuffer.Reset();
		p_InputLayout.Reset();
		p_ConstantBuffer.Reset();
		std::map<std::wstring, ID3D11ShaderResourceView*>(textureList).swap(textureList);
	}

	// テクスチャ読み込み
	bool Texture::LoadTexture(DirectGraphics* p_DirectGraphics_, const std::wstring& fileName_)
	{
		if (textureList.count(fileName_) > 0 && textureList[fileName_] != nullptr) return false;

		if (FAILED(DirectX::CreateWICTextureFromFile(p_DirectGraphics_->GetDevice(), fileName_.c_str(), nullptr, &textureList[fileName_]))) return false;

		return true;
	}

	// テクスチャ解放
	void Texture::ReleaseTexture(const std::wstring& fileName_)
	{
		if (textureList.count(fileName_) != 0 && textureList[fileName_] != nullptr)
		{
			textureList[fileName_]->Release();
			textureList.erase(fileName_);
		}
	}

	// 全てのテクスチャを解放
	void Texture::ReleaseAllTexture()
	{
		for (auto& texture : textureList)
		{
			if (texture.second == nullptr) continue;

			texture.second->Release();
		}

		textureList.clear();
	}

	// テクスチャ描画
	void Texture::DrawTexture(DirectGraphics* p_DirectGraphics_, const std::wstring& fileName_, const float& posX_, const float& posY_, const float& width_, const float& height_, const float angle_)
	{
		UINT strides = sizeof(Utility::TextureVertex);
		UINT offsets = 0;
		ID3D11DeviceContext* p_DeviceContext = p_DirectGraphics_->GetDeviceContext();

		// 入力レイアウトの設定
		p_DeviceContext->IASetInputLayout(p_InputLayout.Get());
		// 頂点シェーダの設定
		p_DeviceContext->VSSetShader(p_DirectGraphics_->GetTextureVertexShader()->GetShaderInterface(), nullptr, 0);
		// インデックスバッファの設定
		p_DeviceContext->IASetIndexBuffer(p_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		// プリミティブ型の設定
		p_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		// 頂点データの設置
		p_DeviceContext->IASetVertexBuffers(0, 1, p_VertexBuffer.GetAddressOf(), &strides, &offsets);
		// ピクセルシェーダのサンプラを設定
		p_DeviceContext->PSSetSamplers(0, 1, p_DirectGraphics_->GetSamplerStateAddress());
		// ピクセルシェーダで使用するテクスチャを設定
		p_DeviceContext->PSSetShaderResources(0, 1, &textureList[fileName_]);
		// ピクセルシェーダの設定
		p_DeviceContext->PSSetShader(p_DirectGraphics_->GetTexturePixelShader()->GetShaderInterface(), nullptr, 0);
		// 出力先の設定
		p_DeviceContext->OMSetRenderTargets(1, p_DirectGraphics_->GetRenderTargetViewAddress(), p_DirectGraphics_->GetDepthStencilView());

		DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(angle_ * (3.14f / 180.0f));
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(width_, height_, 1.0f);
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(posX_, posY_, 0);

		worldMatrix = scale * rotateZ * translate;

		XMStoreFloat4x4(&constantBufferData.world, XMMatrixTranspose(worldMatrix));

		constantBufferData.viewPort = DirectX::XMFLOAT4(p_DirectGraphics_->GetViewPort()->Width, p_DirectGraphics_->GetViewPort()->Height, 0.0f, 1.0f);

		// コンスタントバッファ更新
		p_DeviceContext->UpdateSubresource(p_ConstantBuffer.Get(), 0, NULL, &constantBufferData, 0, 0);

		// コンスタントバッファを設定
		p_DeviceContext->VSSetConstantBuffers(0, 1, p_ConstantBuffer.GetAddressOf());

		// 描画
		p_DeviceContext->DrawIndexed(4, 0, 0);
	}

	// テクスチャを張り付けるポリゴンを作成
    bool Texture::CreatePolygonModel(ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_)
    {
        D3D11_INPUT_ELEMENT_DESC vertexDesc[]
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        // 頂点データ作成
        Utility::TextureVertex vertexList[]
        {
            { { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
            { {  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
            { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
            { {  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
        };

		// 頂点番号
		UWORD vertexNumber[]
		{
			0, 1, 2, 3
		};

		// 頂点バッファ作成
		D3D11_BUFFER_DESC vertexBufferDesc
		{
			.ByteWidth = sizeof(Utility::TextureVertex) * 4,
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_VERTEX_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = 0,
			.StructureByteStride = sizeof(Utility::TextureVertex)
		};

		// 頂点バッファの初期データ
		D3D11_SUBRESOURCE_DATA vertexInitDesc
		{
			.pSysMem = vertexList,
			.SysMemPitch = 0,
			.SysMemSlicePitch = 0
		};

		// バッファ作成
		if (FAILED(p_Device_->CreateBuffer(&vertexBufferDesc, &vertexInitDesc, &p_VertexBuffer))) return false;

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
		if (FAILED(p_Device_->CreateBuffer(&indexBufferDesc, &indexInitData, &p_IndexBuffer))) return false;

		// 頂点レイアウト作成
		if (FAILED(p_Device_->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), p_VertexShader_->GetData(), p_VertexShader_->GetSize(), &p_InputLayout))) return false;

		return true;
    }

	// 定数バッファの作成
	bool Texture::CreateConstantBuffer(DirectGraphics* p_DirectGraphics_)
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
}
