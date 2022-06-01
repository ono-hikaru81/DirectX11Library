
#include "Input.h"
#include <algorithm>

namespace Engine
{
	// 初期化
	bool Input::Initialize()
	{
		// DirectInputインターフェースの初期化
		// DirectInputオブジェクトを取得すると他の入力デバイスを初期化することができる
		HRESULT hr =  DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&p_DirectInput, nullptr);
		if (FAILED(hr)) return false;

		// キーボードのインタフェースを初期化
		hr = p_DirectInput->CreateDevice(GUID_SysKeyboard, &p_KeyboardDevice, nullptr);
		if (FAILED(hr)) return false;

		// キーボードのデータフォーマットの設定
		hr = p_KeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(hr)) return false;

		// キーボードの協調レベルを他プログラムと共有しないように設定
		hr = p_KeyboardDevice->SetCooperativeLevel(FindWindow(TEXT("WindowClass"), nullptr), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if (FAILED(hr)) return false;

		// キーボードを取得
		hr = p_KeyboardDevice->Acquire();
		if (FAILED(hr)) return false;

		return true;
	}

	// 入力状態更新
	void Input::Update()
	{
		BYTE tmpKeyStatus[KEY_MAX]{};
		HRESULT hr = p_KeyboardDevice->GetDeviceState(KEY_MAX, tmpKeyStatus);

		if (SUCCEEDED(hr))
		{
			for (int i = 0; i < KEY_MAX; i++)
			{
				isPushKey = false;

				if (tmpKeyStatus[i] & 0x80)
				{
					isPushKey = true;
				}

				keyStatus[i] = UpdateKeyStatus(isPushKey, keyStatus[i]);
			}
		}
	}
	
	// 解放
	void Input::Release()
	{
		// キーボード解放
		if (p_KeyboardDevice)
		{
			p_KeyboardDevice->Unacquire();
			p_KeyboardDevice->Release();
			p_KeyboardDevice = nullptr;
		}

		// DirectInput解放
		if (p_DirectInput)
		{
			p_DirectInput->Release();
			p_DirectInput = nullptr;
		}
	}

	// キーの押下状態更新
	Input::KeyStatus Input::UpdateKeyStatus(bool isPush_, KeyStatus state_)
	{
		if (isPush_)
		{
			if (state_ == KeyStatus::NoHold)
			{
				return KeyStatus::Pushed;
			}
			else if (state_ == KeyStatus::Pushed || state_ == KeyStatus::Held)
			{
				return KeyStatus::Held;
			}
		}
		else
		{
			if (state_ == KeyStatus::Pushed || state_ == KeyStatus::Held)
			{
				return KeyStatus::Released;
			}
		}

		return KeyStatus::NoHold;
	}
	// キーを押している状態
	bool Input::IsKeyHeld(int key_)
	{
		if (key_ < 0 || key_ >= KEY_MAX) return false;

		return keyStatus[key_] == KeyStatus::Held ? true : false;
	}

	// キーを押した瞬間
	bool Input::IsKeyPushed(int key_)
	{
		if (key_ < 0 || key_ >= KEY_MAX) return false;

		return keyStatus[key_] == KeyStatus::Pushed ? true : false;
	}

	// キーを離した瞬間
	bool Input::IsKeyReleased(int key_)
	{
		if (key_ < 0 || key_ >= KEY_MAX) return false;

		return keyStatus[key_] == KeyStatus::Released ? true : false;
	}
}
