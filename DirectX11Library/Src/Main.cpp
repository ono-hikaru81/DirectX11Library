
#include "Definition.h"
#include "Library/Window/Window.h"
#include "Library/Graphics/DirectGraphics.h"
#include "Library/Input/Input.h"
#include "Library/Sound/Sound.h"

int APIENTRY WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ INT)
{
	Library::Window WindowInfo("DirectX11Library", static_cast<int>(WindowInfo::size.GetX()), static_cast<int>(WindowInfo::size.GetY()));

	if (!WindowInfo.Create()) return 0;

	Library::DirectGraphics directGraphics {};
	Library::Input directInput {};
	Library::Sound directSound {};

	// エンジン初期化
	if (!directGraphics.Initialize()) return 0;
	if (!directInput.Initialize()) return 0;
	if (!directSound.Initialize()) return 0;

	// 画像読み込み
	if (directGraphics.Loadtexture(L"Res/Texture/miniball.png") == false) return 0;

	// 音楽ファイル読み込み
	directSound.LoadFile(Library::Sound::File::TestBGM, "Res/Sound/loop1.wav");

	// メインループ
	while (true)
	{
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			directGraphics.StartRendering();
			directInput.Update();

			if (directInput.IsKeyHeld(DIK_R))
			{
				// 音楽ファイル再生
				directSound.PlayFile(Library::Sound::File::TestBGM, true);
			}

			if (directInput.IsKeyHeld(DIK_S))
			{
				// 音楽ファイル停止
				directSound.StopFile(Library::Sound::File::TestBGM);
			}

			// ポリゴン描画
			directGraphics.DrawPorigon(0.0f, 0.0f, 100.0f, 100.0f);
			directGraphics.DrawRect(100.0f, 100.0f, 100.0f, 100.0f);

			// テクスチャ描画
			directGraphics.DrawTexture(L"Res/Texture/miniball.png");

			directGraphics.FinishRendering();
		}
	}

	// エンジン解放
	directGraphics.Release();
	directInput.Release();
	directSound.Release();

	return 0;
}
