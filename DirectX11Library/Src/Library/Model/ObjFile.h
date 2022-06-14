
#ifndef OBJFILE_H
#define OBJFILE_H

#include <map>
#include <string>
#include <vector>
#include <wrl.h>
#include <d3d11.h>

#include "../Graphics/DirectGraphics.h"
#include "../Shader/VertexShader.h"
#include "../../Utility/Graphics.h"
#include "../../Utility/Vector.h"


namespace Engine
{
	class ObjFile
	{
	public:
		/*
		* @breif コンストラクタ
		*/
		ObjFile() :
			p_VertexBuffer(nullptr),
			p_IndexBuffer(nullptr),
			p_InputLayout(nullptr)
		{
			verticesBuffer.clear();
		}

		/*
		* @breif デストラクタ
		*/
		~ObjFile()
		{
			if (p_VertexBuffer != nullptr)
			{
				p_VertexBuffer->Release();
				p_VertexBuffer = nullptr;
			}

			if (p_IndexBuffer != nullptr)
			{
				p_IndexBuffer->Release();
				p_IndexBuffer = nullptr;
			}

			if (p_InputLayout != nullptr)
			{
				p_InputLayout->Release();
				p_InputLayout = nullptr;
			}
		}

	public:
		/*
		* @breif 読み込み
		*/
		bool Load(const char* fileName_, ID3D11Device*p_Device_, Shader::Vertex* p_VertexShader_);

		/*
		* 描画
		*/
		void Render(DirectGraphics* p_Graphics_, Utility::Vector pos_, Utility::Vector scale_, Utility::Vector degree_);

	private:
		/*
		* @breif メッシュ作成
		*/
		bool CreateMesh(const char* p_FileName_);

		/*
		* @breif 頂点バッファ作成
		*/
		bool CreateVertexBuffer(ID3D11Device* p_Device_);

		/*
		* @breif インデックスバッファ作成
		*/
		bool CreateIndexBuffer(ID3D11Device* p_Device_);

		/*
		* @breif 入力レイアウト作成
		*/
		bool CreateInputLayout(ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_);

		/*
		*/
		void ParseVKeywordTag(std::vector<Utility::Vector>& outVertices_, char* p_Buffer_);

		/*
		*/
		void ParseFKeywordTag(std::vector<Utility::CustomVertex>& outCustomVertices_, std::vector<Utility::Vector>& vertices_, std::vector<Utility::Vector>& normals_, char* p_Buffer_);

		/*
		*/
		void ParseShashKeywordTag(int* p_List_, char* p_Buffer_);

	private:
		void Split(char splitChar, char* p_Buffer, std::vector<std::string>& out);

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> p_VertexBuffer { nullptr };		//! 頂点バッファ(Shader送信用)
		Microsoft::WRL::ComPtr<ID3D11Buffer> p_IndexBuffer{ nullptr };			//! インデックスバッファ(Shader送信用)
		Microsoft::WRL::ComPtr<ID3D11InputLayout> p_InputLayout { nullptr };	//! 入力レイアウト
		std::vector<Utility::CustomVertex> verticesBuffer;						//! 頂点バッファ
		std::vector<UWORD> indexes;												//! インデックスバッファ
	};
}

#endif // !OBJFILE_H
