
#include "ObjFile.h"

#include <stdio.h>
#include <stdlib.h>
#include <codecvt>
#include <cstdio>

#include "../Window/Window.h"

namespace Engine
{
	// 初期化
	bool ObjFile::Initialize(DirectGraphics* p_DirectGraphics_)
	{
		verticesBuffer.clear();

		if (!CreateConstantBuffer(p_DirectGraphics_)) return false;

		SetUpTransform(p_DirectGraphics_);

		return true;
	}

	// 解放
	void ObjFile::Release()
	{
		p_VertexBuffer.Reset();
		p_IndexBuffer.Reset();
		p_InputLayout.Reset();
		p_ConstantBuffer.Reset();
	}

	// 読み込み
	bool ObjFile::Load(const std::string& p_FileName_, ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_)
	{
		if (!CreateMesh(p_FileName_)) return false;

		if (!CreateVertexBuffer(p_Device_)) return false;

		if (!CreateIndexBuffer(p_Device_)) return false;

		if (!CreateInputLayout(p_Device_, p_VertexShader_)) return false;

		return true;
	}

	// 描画
	void ObjFile::Render(DirectGraphics* p_DirectGraphics_, Utility::Vector& pos_, Utility::Vector& scale_, Utility::Vector& degree_)
	{
		p_DirectGraphics_->SetUpDeviceContext();
		UINT strides = sizeof(Utility::ObjFile::Vertex);
		UINT offsets = 0;

		// ワールド行列設定
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(pos_.GetX(), pos_.GetY(), pos_.GetZ());
		DirectX::XMMATRIX rotateX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(degree_.GetX()));
		DirectX::XMMATRIX rotateY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(degree_.GetY()));
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(degree_.GetZ()));
		DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale_.GetX(), scale_.GetY(), scale_.GetZ());
		DirectX::XMMATRIX worldMatrix = scaleMatrix * rotateX * rotateY * rotateZ * translate;

		Utility::ObjFile::ConstantBuffer constantBufferData = p_DirectGraphics_->GetConstantBufferData3D();

		// 入力レイアウトの設定
		p_DirectGraphics_->GetDeviceContext()->IASetInputLayout(p_InputLayout.Get());
		// 頂点バッファの設置
		p_DirectGraphics_->GetDeviceContext()->IASetVertexBuffers(0, 1, p_VertexBuffer.GetAddressOf(), &strides, &offsets);
		// インデックスバッファの設定
		p_DirectGraphics_->GetDeviceContext()->IASetIndexBuffer(p_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		// ワールド行列を定数バッファに設定
		DirectX::XMStoreFloat4x4(&constantBufferData.world, DirectX::XMMatrixTranspose(worldMatrix));
		// 定数バッファ更新
		p_DirectGraphics_->GetDeviceContext()->UpdateSubresource(p_ConstantBuffer.Get(), 0, NULL, &constantBufferData, 0, 0);
		// 定数バッファを設定
		p_DirectGraphics_->GetDeviceContext()->VSSetConstantBuffers(0, 1, p_ConstantBuffer.GetAddressOf());
		// 描画
		p_DirectGraphics_->GetDeviceContext()->DrawIndexed(static_cast<UINT>(indexes.size()), 0, 0);
	}

	// 定数バッファ作成
	bool ObjFile::CreateConstantBuffer(DirectGraphics* p_DirectGraphics_)
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

		if (FAILED(p_DirectGraphics_->GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, p_ConstantBuffer.GetAddressOf()))) return false;

		return true;
	}

	// メッシュ作成
	bool ObjFile::CreateMesh(const std::string& p_FileName_)
	{
		FILE* p_File = nullptr;

		// ファイルを開く
		fopen_s(&p_File, p_FileName_.c_str(), "r");

		if (p_File == nullptr) return false;

		// 頂点座標ベクトル
		std::vector<Utility::Vector> vertices;
		// 法線ベクトル
		std::vector<Utility::Vector> normals;

		// 行の長さ
		const int lineBufferLength = 1024;
		char buffer[lineBufferLength]{};

		// 1行取得
		while (fgets(buffer, lineBufferLength, p_File) != nullptr)
		{
			// コメントを無視する
			if (buffer[0] == '#') continue;

			// スペース解析、取得する
			char* parsePoint = strchr(buffer, ' ');

			if (parsePoint == nullptr) continue;

			int length = static_cast<int>(strlen(buffer));

			// \nを\0に置き換える
			for (int i = 0; i < length; i++)
			{
				if (buffer[i] == '\n')
				{
					buffer[i] = '\0';
				}
			}

			// 頂点関連
			if (buffer[0] == 'v')
			{
				// 頂点座標
				if (buffer[1] == ' ')
				{
					ParseVKeywordTag(vertices, &parsePoint[1]);
				}
				// 法線情報
				else if (buffer[1] == 'n')
				{
					// 文字列"V"を解析
					ParseVKeywordTag(normals, &parsePoint[1]);
				}
			}
			// 多角形面データ
			else if (buffer[0] == 'f')
			{
				// 文字列"F"を解析
				ParseFKeywordTag(verticesBuffer, vertices, normals, &parsePoint[1]);
			}
		}

		// ファイルを閉じる
		fclose(p_File);
		
		return true;
	}

	// 頂点バッファ作成
	bool ObjFile::CreateVertexBuffer(ID3D11Device* p_Device_)
	{
		// 頂点バッファ作成
		D3D11_BUFFER_DESC bufferDesc
		{
			.ByteWidth = sizeof(Utility::ObjFile::Vertex) * static_cast<UINT>(verticesBuffer.size()),	// バッファサイズ
			.Usage = D3D11_USAGE_DEFAULT,			// 使用方法
			.BindFlags = D3D11_BIND_VERTEX_BUFFER,	// BIND設定
			.CPUAccessFlags = 0,					// リソースへのCPUのアクセス権限についての設定
			.MiscFlags = 0,							// リソースオプションのフラグ
			.StructureByteStride = 0				// 構造体のサイズ
		};

		D3D11_SUBRESOURCE_DATA subResourceData
		{
			.pSysMem = &verticesBuffer[0],	// バッファの中身の設定
			.SysMemPitch = 0,				// textureデータを使用する際に使用するメンバ
			.SysMemSlicePitch = 0			// textureデータを使用する際に使用するメンバ
		};

		if (FAILED(p_Device_->CreateBuffer(&bufferDesc, &subResourceData, &p_VertexBuffer))) return false;

		return true;
	}

	// インデックスバッファ作成
	bool ObjFile::CreateIndexBuffer(ID3D11Device* p_Device_)
	{
		// 頂点バッファ作成
		D3D11_BUFFER_DESC bufferDesc
		{
			.ByteWidth = sizeof(UWORD) * static_cast<UINT>(indexes.size()),	// バッファサイズ
			.Usage = D3D11_USAGE_DEFAULT,			// 使用方法
			.BindFlags = D3D11_BIND_INDEX_BUFFER,	// BIND設定
			.CPUAccessFlags = 0,					// リソースへのCPUのアクセス権限についての設定
			.MiscFlags = 0,							// リソースオプションのフラグ
			.StructureByteStride = 0				// 構造体のサイズ
		};

		// 頂点バッファの初期データ
		D3D11_SUBRESOURCE_DATA subResourceData
		{
			.pSysMem = &indexes[0],			// バッファの中身の設定
			.SysMemPitch = 0,				// textureデータを使用する際に使用するメンバ
			.SysMemSlicePitch = 0			// textureデータを使用する際に使用するメンバ
		};

		if (FAILED(p_Device_->CreateBuffer(&bufferDesc, &subResourceData, p_IndexBuffer.GetAddressOf()))) return false;

		return true;
	}

	// 入力レイアウト作成
	bool ObjFile::CreateInputLayout(ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_)
	{
		D3D11_INPUT_ELEMENT_DESC vertexDesc[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		// 頂点レイアウト作成
		if (FAILED(p_Device_->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), p_VertexShader_->GetData(), p_VertexShader_->GetSize(), &p_InputLayout))) return false;

		return true;
	}

	// 頂点座標解析
	void ObjFile::ParseVKeywordTag(std::vector<Utility::Vector>& vertices_, const std::string& p_Buffer_)
	{
		std::vector<std::string> vertexSplit;

		Split(' ', p_Buffer_, vertexSplit);

		int count = 0;
		float values[3] = { 0.0f };

		for (std::string str : vertexSplit)
		{
			values[count] = static_cast<float>(atof(str.c_str()));
			count++;
		}

		vertices_.push_back(Utility::Vector(values[0], values[1], values[2]));
	}

	// 面データ解析
	void ObjFile::ParseFKeywordTag(std::vector<Utility::ObjFile::Vertex>& vertices_, std::vector<Utility::Vector>& pos_, std::vector<Utility::Vector>& normals_, const std::string& p_Buffer_)
	{
		int vertexInfo[3] = { -1, -1, -1 };
		std::vector<std::string> spaceSplit;

		Split(' ', p_Buffer_, spaceSplit);

		for (int i = 0; i < spaceSplit.size(); i++)
		{
			Utility::ObjFile::Vertex vertex;
			ParseSlashKeywordTag(vertexInfo, (spaceSplit[i].c_str()));

			for (int j = 0; j < 3; j++)
			{
				if (vertexInfo[j] == -1) continue;

				int id = vertexInfo[j];

				switch (j)
				{
				case 0: vertex.pos = pos_[id]; break;
				case 2: vertex.normal = normals_[id]; break;
				}
			}

			// 頂点バッファリストに追加
			vertices_.push_back(vertex);

			// インデックスバッファに追加
			indexes.push_back(static_cast<UWORD>(vertices_.size() - 1));
		}

		// 3頂点以上の時
		if (spaceSplit.size() > 3)
		{
			UINT size = static_cast<UINT>(indexes.size());

			indexes.push_back(indexes[size - 4]);
			indexes.push_back(indexes[size - 2]);
		}
	}

	// "/"解析
	void ObjFile::ParseSlashKeywordTag(int* p_List_, const std::string& p_Buffer_)
	{
		int counter = 0;
		std::vector<std::string> slashSplit;

		Split('/', p_Buffer_, slashSplit);

		for (std::string string : slashSplit)
		{
			if (string.size() > 0)
			{
				p_List_[counter] = atoi(string.c_str()) - 1;
			}

			counter++;
		}
	}

	// 変換行列設定
	void ObjFile::SetUpTransform(DirectGraphics* p_DirectGraphics_)
	{
		// ライトの設定
		DirectX::XMVECTOR light = DirectX::XMVector3Normalize(DirectX::XMVectorSet(0.0f, 0.5f, -1.0f, 0.0f));

		Utility::ObjFile::ConstantBuffer constantBufferData = p_DirectGraphics_->GetConstantBufferData3D();

		// コンスタントバッファの設定
		XMStoreFloat4(&constantBufferData.lightVector, light);

		// ライトのカラー設定
		constantBufferData.lightColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		p_DirectGraphics_->SetConstantBufferData3D(constantBufferData);
	}

	// 文字列を分割する
	void ObjFile::Split(const char& splitChar_, const std::string& p_Buffer_, std::vector<std::string>& outString_)
	{
		int count = 0;
		if (p_Buffer_.c_str() == nullptr)
		{
			return;
		}

		int startPoint = 0;

		while (p_Buffer_[count] != '\0')
		{
			if (p_Buffer_[count] == splitChar_)
			{
				if (startPoint != count)
				{
					char splitString[256] = { 0 };
					strncpy_s(splitString, 256, &p_Buffer_[startPoint], static_cast<rsize_t>(count) - startPoint);
					outString_.emplace_back(splitString);
				}
				else
				{
					outString_.emplace_back("");
				}
				startPoint = count + 1;
			}
			count++;
		}

		if (startPoint != count)
		{
			char splitString[256] = { 0 };
			strncpy_s(splitString, 256, &p_Buffer_[startPoint], static_cast<rsize_t>(count) - startPoint);
			outString_.emplace_back(splitString);
		}
	}
}
