
#include "PolygonData.h"

#include "../../Definition.h"
#include "../../Utility/Graphics.h"

namespace Engine
{
	// ポリゴン作成
	bool PolygonData::CreatePorigon(ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_)
	{
		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC vertexDesc[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// 頂点データ作成
		Utility::Porigon2D::Vertex vertexList[] =
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
			.ByteWidth = sizeof(Utility::Porigon2D::Vertex) * 3,
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_VERTEX_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = 0,
			.StructureByteStride = sizeof(Utility::Porigon2D::Vertex)
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
	bool PolygonData::CreateRect(ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_)
	{
		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC vertexDesc[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// 頂点データ作成
		Utility::Porigon2D::Vertex vertexList[]
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
			.ByteWidth = sizeof(Utility::Porigon2D::Vertex) * 4,
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_VERTEX_BUFFER,
			.CPUAccessFlags = 0,
			.MiscFlags = 0,
			.StructureByteStride = sizeof(Utility::Porigon2D::Vertex)
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
};
