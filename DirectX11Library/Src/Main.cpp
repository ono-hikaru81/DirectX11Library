
#include "Definition.h"
//#include "Library/Window/Window.h"
//#include "Library/Graphics/DirectGraphics.h"
//#include "Library/Input/Input.h"
//#include "Library/Sound/Sound.h"
#include "Library/Library.h"

int APIENTRY WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ INT)
{

	// エンジン初期化
	if (!Engine::Engine::Initialize("DirectX11Library", static_cast<int>(WindowInfo::size.GetX()), static_cast<int>(WindowInfo::size.GetY()))) return 0;

	// 画像読み込み
	if (!Engine::Engine::LoadTexture(L"Res/Texture/miniball.png")) return 0;

	// 音楽ファイル読み込み
	Engine::Engine::LoadSoundFile(Engine::Sound::File::TestBGM, "Res/Sound/loop1.wav");

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
			Engine::Engine::StartRendering();
			Engine::Engine::InputUpdate();

			if (Engine::Engine::IsKeyHeld(DIK_R))
			{
				// 音楽ファイル再生
				Engine::Engine::PlaySoundFile(Engine::Sound::File::TestBGM, true);
			}

			if (Engine::Engine::IsKeyHeld(DIK_S))
			{
				// 音楽ファイル停止
				Engine::Engine::StopSoundFile(Engine::Sound::File::TestBGM);
			}

			// ポリゴン描画
			Engine::Engine::DrawPorigon(0.0f, 0.0f, 100.0f, 100.0f);
			Engine::Engine::DrawRect(100.0f, 100.0f, 100.0f, 100.0f);

			// テクスチャ描画
			Engine::Engine::DrawTexture(L"Res/Texture/miniball.png", 0.0f, 0.0f, 32.0f, 32.0f);

			Engine::Engine::FinishRendering();
		}
	}

	// エンジン解放
	Engine::Engine::Release();

	return 0;
}
