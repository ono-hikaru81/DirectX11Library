
#include "PolygonData.h"

#include "../../Definition.h"
#include "../../Utility/Graphics.h"

namespace Library
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
		Utility::Vertex vertexList[] =
		{
			{ { -0.5f,  0.5f, 0.0f, 1.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ {  0.5f, -0.5f, 0.0f, 1.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.5f, -0.5f, 0.0f, 1.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } }
		};

		// 頂点番号
		UWORD vertexNumber[]{ 0, 1, 2 };

		// 頂点バッファ作成
		D3D11_BUFFER_DESC vertexBufferDesc
		{
			vertexBufferDesc.ByteWidth = sizeof(Utility::Vertex) * 3,
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT,
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER,
			vertexBufferDesc.CPUAccessFlags = 0,
			vertexBufferDesc.MiscFlags = 0,
			vertexBufferDesc.StructureByteStride = sizeof(Utility::Vertex)
		};

		// 頂点バッファの初期データ
		D3D11_SUBRESOURCE_DATA vertexInitData
		{
			vertexInitData.pSysMem = vertexList,
			vertexInitData.SysMemSlicePitch = 0,
			vertexInitData.SysMemSlicePitch = 0
		};

		// バッファ作成
		if (FAILED(p_Device_->CreateBuffer(&vertexBufferDesc, &vertexInitData, &p_VertexBuffer))) return false;

		// インデックスバッファ作成
		D3D11_BUFFER_DESC indexBufferDesc
		{
			indexBufferDesc.ByteWidth = sizeof(UWORD) * 3,
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT,
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER,
			indexBufferDesc.CPUAccessFlags = 0,
			indexBufferDesc.MiscFlags = 0,
			indexBufferDesc.StructureByteStride = 0
		};

		// インデックスバッファ作成
		D3D11_SUBRESOURCE_DATA indexInitData
		{
			indexInitData.pSysMem = vertexNumber,
			indexInitData.SysMemPitch = 0,
			indexInitData.SysMemSlicePitch = 0
		};

		// バッファ作成
		if (FAILED(p_Device_->CreateBuffer(&indexBufferDesc, &indexInitData, &p_IndexBuffer))) return false;

		// 入力レイアウト作成
		if (FAILED(p_Device_->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), p_VertexShader_->GetData(), p_VertexShader_->GetSize(), &p_InputLayout))) return false;

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
		Utility::Vertex vertexList[]
		{
			{ { -0.5f,  0.5f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ {  0.5f,  0.5f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.5f, -0.5f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
			{ {  0.5f, -0.5f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } }

		};

		// 頂点番号
		UWORD indexList[]
		{
			0, 1, 2, 1, 3, 2
		};

		// 頂点バッファ作成
		D3D11_BUFFER_DESC vertexBufferDesc
		{
			vertexBufferDesc.ByteWidth = sizeof(Utility::Vertex) * 4,
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT,
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER,
			vertexBufferDesc.CPUAccessFlags = 0,
			vertexBufferDesc.MiscFlags = 0,
			vertexBufferDesc.StructureByteStride = sizeof(Utility::Vertex)
		};

		// 頂点バッファの初期データ
		D3D11_SUBRESOURCE_DATA vertexInitDesc
		{
			vertexInitDesc.pSysMem = vertexList,
			vertexInitDesc.SysMemSlicePitch = 0,
			vertexInitDesc.SysMemSlicePitch = 0
		};

		// バッファ作成
		if (FAILED(p_Device_->CreateBuffer(&vertexBufferDesc, &vertexInitDesc, &p_VertexBuffer))) return false;

		// インデックスバッファ作成
		D3D11_BUFFER_DESC indexBufferDesc
		{
			indexBufferDesc.ByteWidth = sizeof(UWORD) * 6,
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT,
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER,
			indexBufferDesc.CPUAccessFlags = 0,
			indexBufferDesc.MiscFlags = 0,
			indexBufferDesc.StructureByteStride = 0
		};

		// インデックスバッファ作成
		D3D11_SUBRESOURCE_DATA indexInitData
		{
			indexInitData.pSysMem = indexList,
			indexInitData.SysMemPitch = 0,
			indexInitData.SysMemSlicePitch = 0
		};

		// バッファ作成
		if (FAILED(p_Device_->CreateBuffer(&indexBufferDesc, &indexInitData, &p_IndexBuffer))) return false;

		// 頂点レイアウト作成
		if (FAILED(p_Device_->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), p_VertexShader_->GetData(), p_VertexShader_->GetSize(), &p_InputLayout))) return false;

		return true;
	}
};
