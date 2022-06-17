
#include "Sound.h"

#include "../Window/Window.h"

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

namespace Engine
{
	// 初期化
	bool Sound::Initialize()
	{
		// DirectSound生成
		if (FAILED(DirectSoundCreate8(nullptr, &directSound, nullptr))) return false;

		// 協調レベル設定
		if (FAILED(directSound->SetCooperativeLevel(FindWindow(Window::p_ClassName.c_str(), nullptr), DSSCL_NORMAL))) return false;

		for (int i = 0; i < static_cast<int>(Sound::File::Max); i++)
		{
			soundBufferList[i] = nullptr;
		}

		return true;
	}

	// 解放
	void Sound::Release()
	{
		// セカンダリバッファの開放
		for (int i = 0; i < static_cast<int>(File::Max); i++)
		{
			if (soundBufferList[i] != nullptr)
			{
				soundBufferList[i]->Stop();
				soundBufferList[i]->Release();
				soundBufferList[i] = nullptr;
			}
		}

		// DirectSoundインターフェースの解放
		if (directSound != nullptr)
		{
			directSound->Release();
			directSound = nullptr;
		}
	}

	// wavファイル読み込み
	bool Sound::LoadWavFile(const std::string p_FileName_, WavData* p_OutWavData_)
	{
		// WindowsマルチメディアのAPIハンドル
		HMMIO mmioHandle {};

		// チャンク情報
		MMCKINFO ckInfo {};

		// RIFFチャンク用
		MMCKINFO riffCkInfo {};

		mmioHandle = mmioOpen(const_cast<LPSTR>(p_FileName_.c_str()), nullptr, MMIO_READ);

		if (mmioHandle == nullptr) return false;

		// RIFFチャンクに侵入する為にfccにWAVEを設定する
		riffCkInfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');

		// RIFFチャンクに侵入する
		if (MMSYSERR_NOERROR != mmioDescend(mmioHandle, &riffCkInfo, nullptr, MMIO_FINDRIFF))
		{
			// 失敗
			mmioClose(mmioHandle, MMIO_FHOPEN);
			return false;
		}

		// 侵入先のチャンクを"fmt"として設定する
		ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');

		if (MMSYSERR_NOERROR != mmioDescend(mmioHandle, &ckInfo, &riffCkInfo, MMIO_FINDCHUNK))
		{
			// fmtチャンクがない
			mmioClose(mmioHandle, MMIO_FHOPEN);
			return false;
		}

		// fmtデータの読み込み
		LONG readSize { mmioRead(mmioHandle, (HPSTR)&p_OutWavData_->wavFormat, sizeof(p_OutWavData_->wavFormat)) };

		if (readSize != sizeof(p_OutWavData_->wavFormat))
		{
			// 読み込みサイズが一致していないのでエラー
			mmioClose(mmioHandle, MMIO_FHOPEN);
			return false;
		}

		// フォーマットチェック
		if (p_OutWavData_->wavFormat.wFormatTag != WAVE_FORMAT_PCM)
		{
			// フォーマットエラー
			mmioClose(mmioHandle, MMIO_FHOPEN);
			return false;
		}

		// fmtチャンクを退出する
		if (mmioAscend(mmioHandle, &ckInfo, 0) != MMSYSERR_NOERROR)
		{
			mmioClose(mmioHandle, MMIO_FHOPEN);
			return false;
		}

		// dataチャンクに侵入する
		ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		if (mmioDescend(mmioHandle, &ckInfo, &riffCkInfo, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
		{
			// 失敗
			mmioClose(mmioHandle, MMIO_FHOPEN);
			return false;
		}

		// サイズを保存
		p_OutWavData_->size = ckInfo.cksize;

		// dataチャンク読み込み
		p_OutWavData_->soundBuffer = new char[ckInfo.cksize];
		readSize = mmioRead(mmioHandle, (HPSTR)p_OutWavData_->soundBuffer, ckInfo.cksize);

		if (static_cast<unsigned long>(readSize) != ckInfo.cksize)
		{
			mmioClose(mmioHandle, MMIO_FHOPEN);
			delete[] p_OutWavData_->soundBuffer;
			return false;
		}

		// ファイルを閉じる
		mmioClose(mmioHandle, MMIO_FHOPEN);

		return true;
	}

	// ファイル読み込み
	bool Sound::LoadFile(File ID_, const std::string p_FileName_)
	{
		int fileID { static_cast<int>(ID_) };
		WavData wavData;

		if (!LoadWavFile(p_FileName_, &wavData)) return false;

		// バッファ情報の設定
		DSBUFFERDESC bufferDesc {};
		ZeroMemory(&bufferDesc, sizeof(DSBUFFERDESC));
		bufferDesc.dwSize = sizeof(DSBUFFERDESC);
		bufferDesc.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;
		bufferDesc.dwBufferBytes = wavData.size;
		bufferDesc.guid3DAlgorithm = DS3DALG_DEFAULT;
		bufferDesc.lpwfxFormat = &wavData.wavFormat;

		// セカンダリバッファ作成
		if (FAILED(directSound->CreateSoundBuffer(&bufferDesc, &soundBufferList[fileID], nullptr)))
		{
			delete[] wavData.soundBuffer;
			return false;
		}

		// 波形データを書き込むためにセカンダリバッファをロックする
		void* buffer { nullptr };
		DWORD bufferSize { 0 };

		if (FAILED(soundBufferList[fileID]->Lock(0, wavData.size, &buffer, &bufferSize, nullptr, nullptr, 0)))
		{
			delete[] wavData.soundBuffer;
			return false;
		}

		memcpy(buffer, wavData.soundBuffer, bufferSize);

		soundBufferList[fileID]->Unlock(&buffer, bufferSize, nullptr, 0);

		// コピーが終わったらサウンドデータを解放
		delete[] wavData.soundBuffer;

		return true;
	}

	// ファイル再生
	void Sound::PlayFile(File ID_, bool isLoop_)
	{
		int fileID { static_cast<int>(ID_) };

		if (soundBufferList[fileID] == nullptr) return;

		int loopBit { isLoop_ == true ? 1 : 0 };

		// 再生
		soundBufferList[fileID]->Play(0, 0, DSBPLAY_LOOPING & loopBit);
	}

	// ファイル停止
	void Sound::StopFile(File ID_)
	{
		int fileID = static_cast<int>(ID_);

		if (soundBufferList[fileID] == nullptr) return;

		// 停止
		soundBufferList[fileID]->Stop();
		// 再生位置を先頭に戻す
		soundBufferList[fileID]->SetCurrentPosition(0);
	}
}
