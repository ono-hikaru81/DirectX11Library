
#include "Library.h"


namespace Engine
{
	Library* Library::p_Library = nullptr;

	// 初期化
	bool Library::Initialize(const char* p_Title_, unsigned int width_, unsigned int height_)
	{
		p_Library = new Library();

		// ウィンドウ作成
		if (!p_Library->window.Create(p_Title_, width_, height_)) return false;

		// DirectGraphicsの初期化
		if (!p_Library->directGraphics.Initialize()) return false;

		// Inoutの初期化
		if (!p_Library->input.Initialize()) return false;

		// Soundの初期化
		if (!p_Library->sound.Initialize()) return false;

		return true;
	}

	// 解放
	void Library::Release()
	{
		p_Library->directGraphics.Release();
		p_Library->input.Release();
		p_Library->sound.Release();
	}
}
