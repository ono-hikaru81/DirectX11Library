
#ifndef INPUT_H
#define INPUT_H

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <array>

namespace Library
{
	class Input
	{
	private:
		//! キーの状態
		enum class KeyStatus
		{
			NoHold,
			Pushed,
			Held,
			Released
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
		*/
		KeyStatus UpdateKeyStatus(bool isPush_, KeyStatus state_);

	public:
		/**
		* @breif キーを押している
		* @param key_ : 判定するキー
		* @return trueならキーを押し続けている
		*/
		bool IsKeyHeld(int key_);
		/**
		* @breif キーを押した瞬間
		* @param key_ : 判定するキー
		* @return trueならキーを押した瞬間
		*/
		bool IsKeyPushed(int key_);
		/**
		* @breif キーを離した
		* @param key_ : 判定するキー
		* @return trueならキーを離している
		*/
		bool IsKeyReleased(int key_);

	private:
		IDirectInput8* p_DirectInput { 0 };				//! インターフェース
		IDirectInputDevice8* p_KeyboardDevice { 0 };	//! キーボードデバイス

	private:
		bool isPushKey { false };			//! キーが押されているか
		KeyStatus keyStatus[KEY_MAX] {};	//! キー毎の状態を確認する為の配列
	};
}

#endif // !INPUT_H
