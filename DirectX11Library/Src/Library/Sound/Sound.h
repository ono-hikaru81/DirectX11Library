
#ifndef SOUND_H
#define SOUND_H

#include <dsound.h>

namespace Library
{
	/*
	* @breif サウンドを再生する為のクラス
	*/
	class Sound
	{
	public:
		//! サウンドファイル識別
		enum class File
		{
			TestBGM,
			Max
		};

		//! wavデータの構造体
		struct WavData
		{
			WAVEFORMATEX wavFormat;
			char* soundBuffer;
			DWORD size;
		};

	public:
		/**
		* @brief コンストラクタ
		*/
		Sound() = default;

		/**
		* @breif デストラクタ
		*/
		~Sound() = default;

	public:
		/**
		* @breif 初期化関数
		* @return trueなら初期化
		*/
		bool Initialize();

		/**
		* @breif 解放
		*/
		void Release();

	private:
		/*
		* @breif wavファイル読み込み
		* @param p_FIleName_ : 読み込むファイルの名前
		* @param p_OutWavData_ : 読みこんだwavファイルのデータ
		* @return trueなら読み込み成功
		*/
		bool LoadWavFile(const char* p_FileName_, WavData* p_OutWavData_);

	public:
		/**
		* @breif サウンドファイル読み込み
		* @param ID_ : Sound::Fileの列挙子
		* @param p_FIleName_ : 読み込むファイルの名前
		* @return trueなら読み込み成功
		*/
		bool LoadFile(File ID_, const char* p_FileName_);

		/**
		* @breif ファイル再生
		* @param ID_ : Sound::Fileの列挙子
		* @param isLoop_ : ループするか
		*/
		void PlayFile(File ID_, bool isLoop_);

		/**
		* @breif ファイル停止
		* @param ID_ : Sound::Fileの列挙子
		*/
		void StopFile(File ID_);

	private:
		LPDIRECTSOUND8 directSound { 0 };									//! DirectSound
		LPDIRECTSOUNDBUFFER soundBufferList[static_cast<int>(File::Max)];	//! DirectSoundBuffer
	};
}

#endif // !SOUND_H
