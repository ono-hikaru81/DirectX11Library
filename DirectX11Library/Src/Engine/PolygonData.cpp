
#include "PolygonData.h"

#include "../Definition.h"
#include "../Utility/Graphics.h"

// 入力レイアウト
/*
	第1引数 : セマンティック名
		HLSL側の入力シグネチャの名前と一致させることで送信した頂点情報を受信させることができる。

	第2引数 : セマンティック番号
		セマンティック名が同じ場合でも識別できるようにするための番号。

	第3引数 : フォーマット
		データ型やバイト型の設定を行う。
		例えば POSITION と COLOR は DXGI_FORMAT_R32G32B32_FLOAT を設定しているが、
		これは、RGBの各要素を32ビット使えるfloat型という意味なので、
		型やサイズで考えるとfloat[3]と同じ。

	第4引数 : 入力スロット番号
		どの入力スロットに対して入力レイアウトを反映させるかを指定する。
		番号指定された入力スロットは、この入力レイアウトの構成で
		頂点バッファの情報を入力アセンブラに送信します。

	第5引数 : 要素から先頭までのオフセット値
		各データが配列の先頭から何バイト離れているかということ。
		例えば、POSITON は配列の先頭なのでオフセット値は0。
		次の COLOR は POSITION が DXGI_FORMAT_R32G32B32_FLOAT を指定しているので、
		flaot * 3 で12バイトずれた位置からデータが開始されることになるので
		この 12バイト がオフセット値になる。

	第6引数 : 入力データの種類
		頂点データとインスタンスデータがある。

	第7引数 : 繰り返し回数(頂点データの時は0)
		0で問題ない。

*/

D3D11_INPUT_ELEMENT_DESC g_VertexDesc[]
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

namespace DirectX
{
	bool PolygonData::CreatePorigon(ID3D11Device* device_, shader::Vertex* vertexShader_)
	{
		Utility::Vertex vertexList[] =
		{
			{ { -0.5f,  0.5f, 0.0f , 1.0f}, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ {  0.5f, -0.5f, 0.0f , 1.0f}, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.5f, -0.5f, 0.0f , 1.0f}, { 0.0f, 0.0f, 1.0f, 1.0f } }
		};

		UWORD indexList[]{ 0, 1, 2 };

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

		D3D11_SUBRESOURCE_DATA vertexInitData
		{
			vertexInitData.pSysMem = vertexList,
			vertexInitData.SysMemSlicePitch = 0,
			vertexInitData.SysMemSlicePitch = 0
		};

		if (FAILED(device_->CreateBuffer(&vertexBufferDesc, &vertexInitData, &p_VertexBuffer)))
		{
			return false;
		}

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

		D3D11_SUBRESOURCE_DATA indexInitData
		{
			indexInitData.pSysMem = indexList,
			indexInitData.SysMemPitch = 0,
			indexInitData.SysMemSlicePitch = 0
		};

		if (FAILED(device_->CreateBuffer(&indexBufferDesc, &indexInitData, &p_IndexBuffer)))
		{
			return false;
		}

		// 頂点レイアウト作成
		if (FAILED(device_->CreateInputLayout(
			g_VertexDesc,
			ARRAYSIZE(g_VertexDesc),
			vertexShader_->GetData(),
			vertexShader_->GetSize(),
			&p_InputLayout)))
		{
			return false;
		}

		return true;
	}

	bool PolygonData::CreateRect(ID3D11Device* device_, shader::Vertex* vertexShader_)
	{
		Utility::Vertex vertexList[]
		{
			{ { -0.5f,  0.5f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ {  0.5f,  0.5f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.5f, -0.5f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
			{ {  0.5f, -0.5f, 0.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } }

		};

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

		D3D11_SUBRESOURCE_DATA vertexInitDesc
		{
			vertexInitDesc.pSysMem = vertexList,
			vertexInitDesc.SysMemSlicePitch = 0,
			vertexInitDesc.SysMemSlicePitch = 0
		};

		if (FAILED(device_->CreateBuffer(&vertexBufferDesc, &vertexInitDesc, &p_VertexBuffer)))
		{
			return false;
		}

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

		D3D11_SUBRESOURCE_DATA indexInitData
		{
			indexInitData.pSysMem = indexList,
			indexInitData.SysMemPitch = 0,
			indexInitData.SysMemSlicePitch = 0
		};

		if (FAILED(device_->CreateBuffer(&indexBufferDesc, &indexInitData, &p_IndexBuffer)))
		{
			return false;
		}

		// 頂点レイアウト作成
		if (FAILED(device_->CreateInputLayout(
			g_VertexDesc,
			ARRAYSIZE(g_VertexDesc),
			vertexShader_->GetData(),
			vertexShader_->GetSize(),
			&p_InputLayout)))
		{
			return false;
		}

		return true;
	}

	bool PolygonData::CreateCube(ID3D11Device* device_, shader::Vertex* vertexShader_)
	{
		Utility::Vertex vertexList[]
		{
			{ { -0.5f,  0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ {  0.5f,  0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ { -0.5f, -0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ {  0.5f, -0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },

			{ { -0.5f,  0.5f,  0.5f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
			{ { -0.5f, -0.5f,  0.5f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
			{ {  0.5f,  0.5f,  0.5f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },
			{ {  0.5f, -0.5f,  0.5f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } },

			{ { -0.5f,  0.5f,  0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.5f,  0.5f, -0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.5f, -0.5f,  0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.5f, -0.5f, -0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } },

			{ {  0.5f,  0.5f,  0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
			{ {  0.5f, -0.5f,  0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
			{ {  0.5f,  0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
			{ {  0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },

			{ { -0.5f,  0.5f,  0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
			{ {  0.5f,  0.5f,  0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
			{ { -0.5f,  0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },
			{ {  0.5f,  0.5f, -0.5f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f } },

			{ { -0.5f, -0.5f,  0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ {  0.5f, -0.5f,  0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ {  0.5f, -0.5f, -0.5f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		};

		UWORD indexList[]
		{
			 0,  1,  2,  3,  2,  1,
			 4,  5,  6,  7,  6,  5,
			 8,  9, 10, 11, 10,  9,
			12, 13, 14, 15, 14, 13,
			16, 17, 18, 19, 18, 17,
			20, 21, 22, 23, 22, 21,
		};

		// 頂点バッファ作成
		D3D11_BUFFER_DESC vertexBufferDesc
		{
			vertexBufferDesc.ByteWidth = sizeof(Utility::Vertex) * 24,
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT,
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER,
			vertexBufferDesc.CPUAccessFlags = 0,
			vertexBufferDesc.MiscFlags = 0,
			vertexBufferDesc.StructureByteStride = 0
		};

		D3D11_SUBRESOURCE_DATA vertexInitDesc
		{
			vertexInitDesc.pSysMem = vertexList,
			vertexInitDesc.SysMemSlicePitch = 0,
			vertexInitDesc.SysMemSlicePitch = 0
		};

		if (FAILED(device_->CreateBuffer(&vertexBufferDesc, &vertexInitDesc, &p_VertexBuffer)))
		{
			return false;
		}

		// インデックスバッファ作成
		D3D11_BUFFER_DESC indexBufferDesc
		{
			indexBufferDesc.ByteWidth = sizeof(UWORD) * 6 * 6,
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT,
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER,
			indexBufferDesc.CPUAccessFlags = 0,
			indexBufferDesc.MiscFlags = 0,
			indexBufferDesc.StructureByteStride = 0,
		};

		D3D11_SUBRESOURCE_DATA indexInitData
		{
			indexInitData.pSysMem = indexList,
			indexInitData.SysMemPitch = 0,
			indexInitData.SysMemSlicePitch = 0
		};

		if (FAILED(device_->CreateBuffer(&indexBufferDesc, &indexInitData, &p_IndexBuffer)))
		{
			return false;
		}

		// 頂点レイアウト作成
		if (FAILED(device_->CreateInputLayout(
			g_VertexDesc,
			ARRAYSIZE(g_VertexDesc),
			vertexShader_->GetData(),
			vertexShader_->GetSize(),
			&p_InputLayout)))
		{
			return false;
		}

		return true;
	}
};
