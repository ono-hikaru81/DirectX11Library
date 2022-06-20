
#include "Input.h"
#include <algorithm>

namespace Engine
{
	// 初期化
	bool Input::Initialize()
	{
		// DirectInputインターフェースの初期化
		// DirectInputオブジェクトを取得すると他の入力デバイスを初期化することができる
		HRESULT hr { DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)p_DirectInput.GetAddressOf(), nullptr) };
		if (FAILED(hr)) return false;

		// キーボードのインタフェースを初期化
		hr = p_DirectInput.Get()->CreateDevice(GUID_SysKeyboard, p_KeyboardDevice.GetAddressOf(), nullptr);
		if (FAILED(hr)) return false;

		// キーボードのデータフォーマットの設定
		hr = p_KeyboardDevice.Get()->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(hr)) return false;

		// キーボードの協調レベルを他プログラムと共有しないように設定
		hr = p_KeyboardDevice.Get()->SetCooperativeLevel(FindWindow(TEXT("WindowClass"), nullptr), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if (FAILED(hr)) return false;

		// キーボードを取得
		hr = p_KeyboardDevice.Get()->Acquire();
		if (FAILED(hr)) return false;

		return true;
	}

	// 入力状態更新
	void Input::Update()
	{
		BYTE tmpKeyStatus[KEY_MAX] {};
		HRESULT hr { p_KeyboardDevice.Get()->GetDeviceState(KEY_MAX, tmpKeyStatus) };

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
		// DirectInput解放
		p_DirectInput.Reset();

		// キーボード解放
		p_KeyboardDevice.Reset();
	}

	// キーの押下状態更新
	Input::KeyStatus Input::UpdateKeyStatus(const bool& isPush_, const KeyStatus& state_)
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
	bool Input::IsKeyHeld(const int& key_)
	{
		if (key_ < 0 || key_ >= KEY_MAX) return false;

		return keyStatus[key_] == KeyStatus::Held ? true : false;
	}

	// キーを押した瞬間
	bool Input::IsKeyPushed(const int& key_)
	{
		if (key_ < 0 || key_ >= KEY_MAX) return false;

		return keyStatus[key_] == KeyStatus::Pushed ? true : false;
	}

	// キーを離した瞬間
	bool Input::IsKeyReleased(const int& key_)
	{
		if (key_ < 0 || key_ >= KEY_MAX) return false;

		return keyStatus[key_] == KeyStatus::Released ? true : false;
	}
}
