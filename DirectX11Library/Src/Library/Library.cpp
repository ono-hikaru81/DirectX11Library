
#include "Library.h"


namespace Engine
{
	Library* Library::p_Library = nullptr;

	// 初期化
	bool Library::Initialize(const std::string& p_Title_, const unsigned int& width_, const unsigned int& height_)
	{
		p_Library = new Library();

		// ウィンドウ作成
		if (!p_Library->window.Create(p_Title_, width_, height_)) return false;

		// DirectGraphicsの初期化
		if (!p_Library->directGraphics.Initialize()) return false;

		// PolygonDataの初期化
		if (!p_Library->polygon.Initialize(GetDirectGraphics())) return false;

		// Inputの初期化
		if (!p_Library->input.Initialize()) return false;

		// Soundの初期化
		if (!p_Library->sound.Initialize()) return false;

		// Textureの初期化
		if (!p_Library->texture.Initialize(GetDirectGraphics())) return false;

		// カメラの初期化
		p_Library->camera.Initialize(GetDirectGraphics());

		// ObjFileの初期化
		if (!p_Library->objFile.Initialize(GetDirectGraphics())) return false;

		return true;
	}

	// 解放
	void Library::Release()
	{
		p_Library->directGraphics.Release();
		p_Library->polygon.Release();
		p_Library->input.Release();
		p_Library->sound.Release();
		p_Library->texture.Release();
		p_Library->objFile.Release();
	}
}
