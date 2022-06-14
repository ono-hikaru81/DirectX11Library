
#include <stdio.h>
#include <stdlib.h>
#include <codecvt>
#include <cstdio>
#include "ObjFile.h"

namespace Engine
{
	bool ObjFile::Load(const char* p_FileName_, ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_)
	{
		if (!CreateMesh(p_FileName_)) return false;

		if (!CreateVertexBuffer(p_Device_)) return false;

		if (!CreateIndexBuffer(p_Device_)) return false;

		if (!CreateInputLayout(p_Device_, p_VertexShader_)) return false;

		return true;
	}

	void ObjFile::Render(DirectGraphics* p_Graphics_, Utility::Vector pos_, Utility::Vector scale_, Utility::Vector degree_)
	{
		p_Graphics_->SetUpDeviceContext();
		ID3D11Buffer* constantBuffer = p_Graphics_->GetObjFileConstantBuffer();
		UINT strides = sizeof(Utility::CustomVertex);
		UINT offsets = 0;

		// ワールド行列設定
		DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(pos_.GetX(), pos_.GetY(), pos_.GetZ());
		DirectX::XMMATRIX rotateX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(degree_.GetX()));
		DirectX::XMMATRIX rotateY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(degree_.GetY()));
		DirectX::XMMATRIX rotateZ = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(degree_.GetZ()));
		DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(scale_.GetX(), scale_.GetY(), scale_.GetZ());
		DirectX::XMMATRIX worldMatrix = scaleMatrix * rotateX * rotateY * rotateZ * translate;

		// 入力レイアウトの設定
		p_Graphics_->GetDeviceContext()->IASetInputLayout(p_InputLayout.Get());
		// 頂点バッファの設置
		p_Graphics_->GetDeviceContext()->IASetVertexBuffers(0, 1, p_VertexBuffer.GetAddressOf(), &strides, &offsets);
		// インデックスバッファの設定
		p_Graphics_->GetDeviceContext()->IASetIndexBuffer(p_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		// ワールド行列を定数バッファに設定
		DirectX::XMStoreFloat4x4(&p_Graphics_->GetObjFileConstantBufferData()->world, DirectX::XMMatrixTranspose(worldMatrix));
		// 定数バッファ更新
		p_Graphics_->GetDeviceContext()->UpdateSubresource(p_Graphics_->GetObjFileConstantBuffer(), 0, NULL, p_Graphics_->GetObjFileConstantBufferData(), 0, 0);
		// 定数バッファを設定
		p_Graphics_->GetDeviceContext()->VSSetConstantBuffers(0, 1, &constantBuffer);
		// 描画
		p_Graphics_->GetDeviceContext()->DrawIndexed(static_cast<UINT>(indexes.size()), 0, 0);
	}

	bool ObjFile::CreateMesh(const char* p_FileName_)
	{
		FILE* p_File = nullptr;

		fopen_s(&p_File, p_FileName_, "r");

		if (p_File == nullptr) return false;

		std::vector<Utility::Vector> vertices;
		std::vector<Utility::Vector> normals;

		const int lineBufferLength = 1024;
		char buffer[lineBufferLength]{};

		while (fgets(buffer, lineBufferLength, p_File) != nullptr)
		{
			// コメントを無視する
			if (buffer[0] == '#') continue;

			char* parsePoint = strchr(buffer, ' ');
			if (parsePoint == nullptr) continue;

			int length = static_cast<int>(strlen(buffer));

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
					ParseVKeywordTag(normals, &parsePoint[1]);
				}
			}
			else if (buffer[0] == 'f')
			{
				ParseFKeywordTag(verticesBuffer, vertices, normals, &parsePoint[1]);
			}
		}

		fclose(p_File);
		
		return true;
	}

	bool ObjFile::CreateVertexBuffer(ID3D11Device* p_Device_)
	{
		// 頂点バッファ作成
		D3D11_BUFFER_DESC bufferDesc
		{
			.ByteWidth = sizeof(Utility::CustomVertex) * static_cast<UINT>(verticesBuffer.size()),	// バッファサイズ
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
			.pSysMem = &indexes[0],	// バッファの中身の設定
			.SysMemPitch = 0,				// textureデータを使用する際に使用するメンバ
			.SysMemSlicePitch = 0			// textureデータを使用する際に使用するメンバ
		};

		if (FAILED(p_Device_->CreateBuffer(&bufferDesc, &subResourceData, p_IndexBuffer.GetAddressOf()))) return false;

		return true;
	}

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

	void ObjFile::ParseVKeywordTag(std::vector<Utility::Vector>& outVertices_, char* p_Buffer_)
	{
		std::vector<std::string> splitStrings;

		Split(' ', p_Buffer_, splitStrings);

		int count = 0;
		float values[3] = { 0.0f };

		for (std::string str : splitStrings)
		{
			values[count] = static_cast<float>(atof(str.c_str()));
			count++;
		}

		outVertices_.push_back(Utility::Vector(values[0], values[1], values[2]));
	}

	void ObjFile::ParseFKeywordTag(std::vector<Utility::CustomVertex>& outCustomVertices_, std::vector<Utility::Vector>& vertices_, std::vector<Utility::Vector>& normals_, char* p_Buffer_)
	{
		int vertexInfo[3] = { -1, -1, -1 };
		std::vector<std::string> spaceSplit;

		Split(' ', p_Buffer_, spaceSplit);

		for (int i = 0; i < spaceSplit.size(); i++)
		{
			Utility::CustomVertex vertex;
			ParseShashKeywordTag(vertexInfo, const_cast<char*>(spaceSplit[i].c_str()));

			for (int j = 0; j < 3; j++)
			{
				if (vertexInfo[j] == -1) continue;

				int id = vertexInfo[j];

				switch (j)
				{
				case 0: vertex.pos = vertices_[id]; break;
				case 2: vertex.normal = normals_[id]; break;
				}
			}

			// 頂点バッファリストに追加
			outCustomVertices_.push_back(vertex);

			// インデックスバッファに追加
			indexes.push_back(static_cast<UWORD>(outCustomVertices_.size() - 1));
		}
	}

	void ObjFile::ParseShashKeywordTag(int* p_List_, char* p_Buffer_)
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

	void ObjFile::Split(char splitChar, char* p_Buffer, std::vector<std::string>& out)
	{
		int count = 0;
		if (p_Buffer == nullptr)
		{
			return;
		}

		int start_point = 0;

		while (p_Buffer[count] != '\0')
		{
			if (p_Buffer[count] == splitChar)
			{
				if (start_point != count)
				{
					char split_str[256] = { 0 };
					strncpy_s(split_str, 256, &p_Buffer[start_point], count - start_point);
					out.emplace_back(split_str);
				}
				else
				{
					out.emplace_back("");
				}
				start_point = count + 1;
			}
			count++;
		}

		if (start_point != count)
		{
			char split_str[256] = { 0 };
			strncpy_s(split_str, 256, &p_Buffer[start_point], count - start_point);
			out.emplace_back(split_str);
		}
	}

}
