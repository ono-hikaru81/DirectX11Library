
#ifndef POLYGON_H
#define POLYGON_H

#include <d3d11.h>
#include <wrl.h>

#include "../Graphics/DirectGraphics.h"
#include "../Shader/VertexShader.h"

namespace Engine
{
	/*
	* @breif ポリゴンを作成するクラス
	*/
	class Polygon
	{
	public:
		/**
		* @breif コンストラクタ
		*/
		Polygon() = default;

		/**
		* @breif デストラクタ
		*/
		~Polygon() = default;

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

		/**
		* @breif 三角形描画
		* @param posX_ : 左上X座標
		* @param posY_ : 左上Y座標
		* @param width_ : X軸の大きさ
		* @param height_ : Y軸の大きさ
		* @param angle_ : 角度
		*/
		void RenderTriangle(DirectGraphics* p_DirectGraphics_, const float& posX_, const float& posY_, const float& width_, const float& height_, const float angle_ = 0.0f,
							const Utility::Vector color_ = Utility::Vector(0.0f, 0.0f, 0.0f));

		/**
		* @breif 矩形描画
		* @param posX_ : 左上X座標
		* @param posY_ : 左上Y座標
		* @param width_ : X軸の大きさ
		* @param height_ : Y軸の大きさ
		* @param angle_ : 角度
		*/
		void RenderRect(DirectGraphics* p_DirectGraphics_, const float& posX_, const float& posY_, const float& width_, const float& height_, const float angle_ = 0.0f,
						const Utility::Vector color_ = Utility::Vector(0.0f, 0.0f, 0.0f));

	private:
		/**
		* @breif 三角形作成
		* @param p_Device_ : デバイス
		* @param p_VertexShader_ : 頂点シェーダ
		* @return trueなら作成
		*/
		bool CreateTriangleData(ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_);
		/**
		* @breif 矩形作成
		* @param p_Device_ : デバイス
		* @param p_VertexShader_ : 頂点シェーダ
		* @return trueなら作成
		*/
		bool CreateRectData(ID3D11Device* p_Device_, Shader::Vertex* p_VertexShader_);

		/**
		* @breif 定数バッファの作成
		* @return trueなら作成
		*/
		bool CreateConstantBuffer(DirectGraphics* p_DirectGraphics_);

	private:
		static Polygon* p_Triangle;	//! 三角形用インスタンス
		static Polygon* p_Rect;		//! 矩形用インスタンス

		Microsoft::WRL::ComPtr<ID3D11Buffer> p_VertexBuffer { nullptr };		//! 頂点バッファ
		Microsoft::WRL::ComPtr<ID3D11Buffer> p_IndexBuffer { nullptr };			//! インデックスバッファ
		Microsoft::WRL::ComPtr<ID3D11InputLayout> p_InputLayout { nullptr };	//! 入力レイアウト
		Microsoft::WRL::ComPtr<ID3D11Buffer> p_ConstantBuffer { nullptr };		//! 定数バッファ
		Utility::Porygon2D::ConstantBuffer constantBufferData {};				//! 定数バッファのデータ
	};
};

#endif // !POLYGON_H
