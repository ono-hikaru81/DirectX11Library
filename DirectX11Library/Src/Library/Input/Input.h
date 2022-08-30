
#ifndef INPUT_H
#define INPUT_H

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <array>
#include <dinput.h>
#include <wrl.h>

namespace Engine
{
	/*
	* 入力する機能を実装するクラス
	*/
	class Input
	{
	private:
		//! キーの状態
		enum class KeyStatus
		{
			NoHold,		//! 押していない
			Pushed,		//! 押した瞬間
			Held,		//! 押している
			Released	//! 離した瞬間
		};

	private:
		static constexpr int KEY_MAX{ 256 };	//! キーの最大数

	public:
		/**
		* @breif コンストラクタ
		*/
		Input() = default;

		/**
		* @breif デストラクタ
		*/
		~Input() = default;

	public:
		/**
		* @breif 初期化
		* @return trueが帰ってきたら初期化
		*/
		bool Initialize();

		/**
		* @breif 入力状態更新
		*/
		void Update();

		/**
		* @breif 解放
		*/
		void Release();

	private:
		/**
		* @breif キーの押下状態更新
		* @param isPush_ : キーを押しているか
		* @param state_ : キーの押下状態
		*/
		KeyStatus UpdateKeyStatus(const bool& isPush_, const KeyStatus& state_);

	public:
		/**
		* @breif キーを押している
		* @param key_ : 判定するキー
		* @return trueならキーを押し続けている
		*/
		bool IsKeyHeld(const int& key_);
		/**
		* @breif キーを押した瞬間
		* @param key_ : 判定するキー
		* @return trueならキーを押した瞬間
		*/
		bool IsKeyPushed(const int& key_);
		/**
		* @breif キーを離した
		* @param key_ : 判定するキー
		* @return trueならキーを離している
		*/
		bool IsKeyReleased(const int& key_);

	private:
		Microsoft::WRL::ComPtr<IDirectInput8> p_DirectInput { nullptr };			//! DirectInput
		Microsoft::WRL::ComPtr<IDirectInputDevice8> p_KeyboardDevice { nullptr };	//! キーボードデバイス

		bool isPushKey { false };			//! キーが押されているか
		KeyStatus keyStatus[KEY_MAX] {};	//! キー毎の状態を確認する為の配列
	};
}

#endif // !INPUT_H
