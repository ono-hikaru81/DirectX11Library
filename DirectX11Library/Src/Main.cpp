
#include "Definition.h"
#include "Engine/DirectGraphics.h"
#include "Engine/PolygonData.h"
#include "Window/Window.h"

int APIENTRY WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ INT)
{
	Window WindowParam("DirectX11Library", static_cast<int>(WindowParam::size.GetX()), static_cast<int>(WindowParam::size.GetY()));

	if (WindowParam.Create() == false) return 0;

	DirectX::Graphics dg {};

	if (dg.Init() == false) return 0;

	// モデル初期化
//ModelData* p_model;

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
			dg.StartRendering();

			dg.DrawPorigon(0.0f, 0.0f, 100.0f, 100.0f);
			dg.DrawRect(100.0f, 100.0f, 100.0f, 100.0f);
			dg.DrawCube(200.0f, 200.0f, 100.0f, 100.0f);

			dg.FinishRendering();
		}
	}

	return 0;
}
