
#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <string>

namespace Engine
{
	/**
	* @breif Windowを生成する為のクラス
	*/
	class Window
	{
	public:
		static const std::string p_ClassName;

	private:
		/**
		* @breif ウィンドウプロシージャ
		*/
		static LRESULT CALLBACK WindowProcedure(HWND windowHandle_, UINT messageID_, WPARAM wparam_, LPARAM lparam_);

	public:
		/**
		* @breif コンストラクタ
		*/
		Window() = default;

		/**
		* @breif デストラクタ
		*/
		~Window() = default;

		/**
		* @breif ウィンドウ生成
		* @return trueが帰ってきたら作成
		*/
		bool Create(const std::string& p_Title_, const unsigned int& width_, const unsigned int& height_);

	private:
		/**
		* @breif ウィンドウクラスの登録
		* @return trueが帰ってきたら登録
		* @detail RegisterClass関数を実行する関数
		*/
		bool RegisterWindowClass();

		/*
		* @breif ウィンドウのサイズ調整
		* @param windowHandle_ : ウィンドウハンドル
		* @detail ウィンドウ枠とクライアント領域の調整
		*/
		void ResizeWindow(HWND& windowHandle_);

	private:
		std::string p_Title { "" };	//! タイトル名
		unsigned int width { 0 };	//! 横幅
		unsigned int height { 0 };	//! 縦幅
	};
}

#endif // !WINDOW_H
