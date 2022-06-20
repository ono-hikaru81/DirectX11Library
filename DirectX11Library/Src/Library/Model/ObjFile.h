
#ifndef OBJFILE_H
#define OBJFILE_H

#include <d3d11.h>
#include <map>
#include <string>
#include <vector>
#include <wrl.h>

#include "../Graphics/DirectGraphics.h"
#include "../Shader/VertexShader.h"
#include "../../Utility/Graphics.h"
#include "../../Utility/Vector.h"


namespace Engine
{
	/*
	* ObjFileを描画するためのクラス
	*/
	class ObjFile
	{
	public:
		/*
		* @breif コンストラクタ
		*/
		ObjFile() = default;

		/*
		* @breif デストラクタ
		*/
		~ObjFile() = default;

	public:
		/**
		* @breif 初期化
		* @return trueなら初期化成功
		*/
		bool Initialize(DirectGraphics* p_DirectGraphics_);

		/**
		* @breif 解放
		*/
		void Release();

		/*
		* @breif 読み込み
		* @param fileName_ : 読みこむオブジェファイルの名前
		* @param p_Device_ : デバイス
		* @param p_VertexShader_ : 頂点シェーダ
		* @return trueなら読み込み成功
		*/
		bool Load(const std::string& fileName_, ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_);

		/*
		* @breif 描画
		* @param pos_ : 座標
		* @param scale_ : 拡縮
		* @param degree_ : 回転
		*/
		void Render(DirectGraphics* p_DirectGraphics_, Utility::Vector& pos_, Utility::Vector& scale_, Utility::Vector& degree_);

	private:
		/*
		* @breif 定数バッファ作成
		* @return tureなら作成成功
		*/
		bool CreateConstantBuffer(DirectGraphics* p_DirectGraphics_);

		/*
		* @breif メッシュ作成
		* @return tureなら作成成功
		*/
		bool CreateMesh(const std::string& p_FileName_);

		/*
		* @breif 頂点バッファ作成
		* @param p_Device_ : デバイス
		* @return tureなら作成成功
		*/
		bool CreateVertexBuffer(ID3D11Device* p_Device_);

		/*
		* @breif インデックスバッファ作成
		* @param p_Device_ : デバイス
		* @return trueなら作成成功
		*/
		bool CreateIndexBuffer(ID3D11Device* p_Device_);

		/*
		* @breif 入力レイアウト作成
		* @param p_Device_ : デバイス
		* @param p_VertexShader_ : 頂点シェーダ
		* @return trueなら作成成功
		*/
		bool CreateInputLayout(ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_);

		/*
		* @breif 頂点座標解析
		* @param vertices_ : 頂点リスト
		* @param p_Buffer_ :　頂点データの一行
		*/
		void ParseVKeywordTag(std::vector<Utility::Vector>& vertices_, const std::string& p_Buffer_);

		/*
		* @breif 面データ解析
		* @param vertices_ : 頂点リスト
		* @param pos_ : 座標
		* @param normals_ : 法線
		* @param p_Buffer_ : 面データの一行
		*/
		void ParseFKeywordTag(std::vector<Utility::ObjFile::Vertex>& vertices_, std::vector<Utility::Vector>& pos_, std::vector<Utility::Vector>& normals_, const std::string& p_Buffer_);

		/*
		* @breif "/"解析
		* @param p_List_ : 区切られている文字数
		* @param p_Buffer_ : 区切りされている文字列
		*/
		void ParseSlashKeywordTag(int* p_List_, const std::string& p_Buffer_);

		/*
		* @breif 変換行列設定
		*/
		void SetUpTransform();

		/*
		* @brief 文字列を分割する
		* @param splitChar : 分割する文字
		* @param p_Buffer : 分割される文字列
		* @param out_ : 分割した文字
		*/
		void Split(const char& splitChar_, const std::string& p_Buffer_, std::vector<std::string>& outString_);

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> p_VertexBuffer { nullptr };		//! 頂点バッファ(Shader送信用)
		Microsoft::WRL::ComPtr<ID3D11Buffer> p_IndexBuffer { nullptr };			//! インデックスバッファ(Shader送信用)
		Microsoft::WRL::ComPtr<ID3D11InputLayout> p_InputLayout { nullptr };	//! 入力レイアウト
		Microsoft::WRL::ComPtr<ID3D11Buffer> p_ConstantBuffer { nullptr };		//! 定数バッファ
		Utility::ObjFile::ConstantBuffer constantBufferData{};					//! 定数バッファのデータ
		std::vector<Utility::ObjFile::Vertex> verticesBuffer;					//! 頂点バッファ
		std::vector<UWORD> indexes;												//! インデックスバッファ
	};
}

#endif // !OBJFILE_H
