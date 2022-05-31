
#include "Window.h"

namespace Library
{
	const char* Window::p_ClassName = "WindowClass";

	// ウィンドウプロシージャ
	LRESULT CALLBACK Window::WindowProcedure(HWND windowHandle_, UINT messageID_, WPARAM wParam_, LPARAM lParam_)
	{
		switch (messageID_)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(windowHandle_, messageID_, wParam_, lParam_);
			break;
		}

		return 0;
	}

	// ウィンドウ作成
	bool Window::Create()
	{
		if (!RegisterWindowClass()) return 0;

		HWND windowHandle = CreateWindow(
			Window::p_ClassName,
			p_Title,
			(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) | WS_VISIBLE,
			CW_USEDEFAULT,
			0,
			width,
			height,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			NULL);

		if (windowHandle == NULL) return 0;

		ResizeWindow(windowHandle);

		ShowWindow(windowHandle, SW_SHOW);

		UpdateWindow(windowHandle);

		return true;
	}

	// ウィンドウクラス登録
	bool Window::RegisterWindowClass()
	{
		WNDCLASSEX windowClass = {
			sizeof(WNDCLASSEX),
			CS_HREDRAW | CS_VREDRAW,
			WindowProcedure,
			0,
			0,
			GetModuleHandle(NULL),
			LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION)),
			LoadCursor(NULL, IDC_ARROW),
			NULL,
			NULL,
			Window::p_ClassName,
			NULL
		};

		if (RegisterClassEx(&windowClass) == 0) return false;

		return true;
	}

	// ウィンドウのサイズ調整
	void Window::ResizeWindow(HWND windowHandle_)
	{
		RECT windowRect;
		RECT clientRect;

		GetWindowRect(windowHandle_, &windowRect);
		GetClientRect(windowHandle_, &clientRect);

		int frameSizeX = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left);
		int frameSizeY = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top);

		SetWindowPos(windowHandle_, NULL, CW_USEDEFAULT, 0, frameSizeX + width, frameSizeY + height, SWP_NOMOVE);
	}

}
