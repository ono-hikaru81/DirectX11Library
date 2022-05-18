
#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>


namespace Library
{
	/**
* @breif Windowを生成する為のクラス
*/
	class Window
	{
	public:
		static const char* p_ClassName;

	private:
		/**
		* @breif ウィンドウプロシージャ
		*/
		static LRESULT CALLBACK WindowProcedure(HWND windowHandle_, UINT messageID_, WPARAM wparam_, LPARAM lparam_);

	public:
		/**
		* @breif コンストラクタ
		* @param p_Title_ : タイトル名
		* @param width_ : 横幅
		* @param height_ : 縦幅
		*/
		Window(const char* p_Title_, unsigned int width_, unsigned int height_) :
			p_Title(p_Title_),
			width(width_),
			height(height_)
		{}

		/**
		* @breif デストラクタ
		*/
		~Window() {}

		/**
		* @breif ウィンドウ生成
		* @return trueが帰ってきたら作成
		*/
		bool Create();

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
		void ResizeWindow(HWND windowHandle_);

	private:
		const char* p_Title;	//! タイトル名
		unsigned int width;		//! 横幅
		unsigned int height;	//! 縦幅
	};
}

#endif // !WINDOW_H
