
#include "Definition.h"
#include "Library/Library.h"
#include <time.h>

int APIENTRY WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ INT)
{

	// エンジン初期化
	if (!Engine::Library::Initialize("DirectX11Library", static_cast<int>(WindowInfo::size.GetX()), static_cast<int>(WindowInfo::size.GetY()))) return 0;

	// 画像読み込み
	if (!Engine::Library::LoadTexture(L"Res/Texture/miniball.png")) return 0;

	// 音楽ファイル読み込み
	Engine::Library::LoadSoundFile(Engine::Sound::File::TestBGM, "Res/Sound/loop1.wav");

	// objファイル読み込み
	if(!Engine::Library::LoadObjFile("Res/Model/hikouki.obj")) return 0;

	Utility::Vector pos = Utility::Vector(0.0f, 0.0f, 0.0f);
	Utility::Vector degree = Utility::Vector(0.0f, 0.0f, 0.0f);
	Utility::Vector scale = Utility::Vector(1.0f, 1.0f, 1.0f);

	timeBeginPeriod(1);

	DWORD current = timeGetTime();
	DWORD before = current;

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
			Engine::Library::StartRendering();
			Engine::Library::InputUpdate();

			if (Engine::Library::IsKeyHeld(DIK_R))
			{
				// 音楽ファイル再生
				Engine::Library::PlaySoundFile(Engine::Sound::File::TestBGM, true);
			}

			if (Engine::Library::IsKeyHeld(DIK_S))
			{
				// 音楽ファイル停止
				Engine::Library::StopSoundFile(Engine::Sound::File::TestBGM);
			}

			// ポリゴン描画
			Engine::Library::DrawPorigon(0.0f, 0.0f, 100.0f, 100.0f);
			Engine::Library::DrawRect(100.0f, 100.0f, 100.0f, 100.0f);

			// テクスチャ描画
			Engine::Library::DrawTexture(L"Res/Texture/miniball.png", 0.0f, 0.0f, 32.0f, 32.0f);

			degree.operator+=(Utility::Vector(0.0f, 1.0f, 0.0f));
			// objファイル描画
			Engine::Library::RenderObjFile(pos, scale, degree);

			Engine::Library::FinishRendering();

			while (current - before < 17)
			{
				Sleep(current - before);

				current = timeGetTime();
			}

			before = current;
		}
	}

	timeEndPeriod(1);

	// エンジン解放
	Engine::Library::Release();

	return 0;
}
