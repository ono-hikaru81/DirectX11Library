
#ifndef LIBRARY_H
#define LIBRARY_H

#include "Graphics/DirectGraphics.h"
#include "Input/Input.h"
#include "Model/ObjFile.h"
#include "Shader/VertexShader.h"
#include "Sound/Sound.h"
#include "Window/Window.h"

namespace Engine
{
	class Library
	{
	public:
		/*
		* @brief コンストラクタ
		*/
		Library() = default;

		/*
		* @brief デストラクタ
		*/
		~Library() = default;

	public:
		/*
		* @breif 初期化
		* @return trueなら初期化成功
		*/
		static bool Initialize(const char* p_Title_, unsigned int width_, unsigned int height_);

		/*
		* @breif 解放
		*/
		static void Release();

	public:
		//================
		// DirectGraphics
		//================
		/**
		* @breif 描画開始
		*/
		static void StartRendering() { p_Library->directGraphics.StartRendering(); }

		/**
		* @breif 描画終了
		*/
		static void FinishRendering() { p_Library->directGraphics.FinishRendering(); }

		/**
		* @breif ポリゴン描画
		* @param posX_ : 左上X座標
		* @param posY_ : 左上Y座標
		* @param width_ : X軸の大きさ
		* @param height_ : Y軸の大きさ
		* @param angle_ : 角度
		*/
		static void DrawPorigon(float posX_, float posY_, float width_, float height_, float angle_ = 0.0f) { p_Library->directGraphics.DrawPorigon(posX_, posY_, width_, height_, angle_); }

		/**
		* @breif 矩形描画
		* @param posX_ : 左上X座標
		* @param posY_ : 左上Y座標
		* @param width_ : X軸の大きさ
		* @param height_ : Y軸の大きさ
		* @param angle_ : 角度
		*/
		static void DrawRect(float posX_, float posY_, float width_, float height_, float angle_ = 0.0f) { p_Library->directGraphics.DrawRect(posX_, posY_, width_, height_, angle_); }

		/*
		* @brief テクスチャ読み込み
		* @return trueなら読み込み成功
		* @param fileName_ : 読みこむテクスチャファイルの名前
		*/
		static bool LoadTexture(const std::wstring fileName_) 
		{
			if (!p_Library->directGraphics.LoadTexture(fileName_)) return false;

			return true;
		}

		/*
		* @breif テクスチャ解放
		* @param fileName_ : 解放するテクスチャファイルの名前
		*/
		static void ReleaseTexture(const std::wstring fileName_) { p_Library->directGraphics.ReleaseTexture(fileName_); }

		/*
		* @breif 全てのテクスチャを解放
		*
		*/
		static void ReleaseAllTexture() { p_Library->directGraphics.ReleaseAllTexture(); }

		/*
		* @breif テクスチャ描画
		* @param fileName_ : 読みこむテクスチャファイルの名前
		*/
		static void DrawTexture(const std::wstring fileName_, float posX_, float posY_, float width_, float height_, float angle_ = 0.0f) 
		{
			p_Library->directGraphics.DrawTexture(fileName_, posX_, posY_, width_, height_, angle_);
		}

		//=======
		// Input
		//=======
		/**
		* @breif 入力状態更新
		*/
		static void InputUpdate() { p_Library->input.Update(); }
		/**
		* @breif キーを押している
		* @param key_ : 判定するキー
		* @return trueならキーを押し続けている
		*/
		static bool IsKeyHeld(int key_) 
		{ 
			if (!p_Library->input.IsKeyHeld(key_)) return false;

			return true;
		}
		/**
		* @breif キーを押した瞬間
		* @param key_ : 判定するキー
		* @return trueならキーを押した瞬間
		*/
		static bool IsKeyPushed(int key_) 
		{ 
			if (!p_Library->input.IsKeyPushed(key_)) return false;

			return true;
		}
		/**
		* @breif キーを離した
		* @param key_ : 判定するキー
		* @return trueならキーを離している
		*/
		static bool IsKeyReleased(int key_) 
		{ 
			if (!p_Library->input.IsKeyReleased(key_)) return false;

			return true;
		}

		//=======
		// Sound
		//=======
		/**
		* @breif サウンドファイル読み込み
		* @param ID_ : Sound::Fileの列挙子
		* @param p_FIleName_ : 読み込むファイルの名前
		* @return trueなら読み込み成功
		*/
		static bool LoadSoundFile(Sound::File ID_, const char* p_FileName_)
		{ 
			if (!p_Library->sound.LoadFile(ID_, p_FileName_)) return false;

			return true;
		}

		/**
		* @breif ファイル再生
		* @param ID_ : Sound::Fileの列挙子
		* @param isLoop_ : ループするか
		*/
		static void PlaySoundFile(Sound::File ID_, bool isLoop_) { p_Library->sound.PlayFile(ID_, isLoop_); }

		/**
		* @breif ファイル停止
		* @param ID_ : Sound::Fileの列挙子
		*/
		static void StopSoundFile(Sound::File ID_) { p_Library->sound.StopFile(ID_); }

		/*
		* pbreif オブジェファイル読み込み
		*/
		static bool LoadObjFile(const char* fileName_) 
		{ 
			if (!p_Library->objFile.Load(fileName_, GetDirectGraphics()->GetDevice(), GetDirectGraphics()->GetVertexShader())) return false;

			return true;
		}

		/*
		* @breif オブジェファイル描画
		*/
		static void RenderObjFile(Utility::Vector pos_, Utility::Vector scale_, Utility::Vector degree_) 
		{ 
			p_Library->objFile.Render(GetDirectGraphics(), pos_, scale_, degree_);
		}

	private:
		static DirectGraphics* GetDirectGraphics() { return &p_Library->directGraphics; }

	private:
		static Library* p_Library;

		Window window {};					//! ウィンドウクラス
		DirectGraphics directGraphics {};	//! ダイレクトグラフィックスクラス
		Input input {};						//! インプットクラス
		Sound sound {};						//! サウンドクラス
		ObjFile objFile {};					//! オブジェファイルクラス
	};
}

#endif // !LIBRARY_H
