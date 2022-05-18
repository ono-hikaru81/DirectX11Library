
#include "ShaderBase.h"

#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

namespace Library
{
	namespace Shader
	{
		// シェーダファイル読み込み
		int Base::LoadFile(const char* p_FileName_)
		{
			FILE* fp = nullptr;

			// ファイルを開く
			fopen_s(&fp, p_FileName_, "rb");

			if (fp == nullptr) return 0;

			// ファイルポインタの位置をファイルの終わりに移動
			fseek(fp, 0, SEEK_END);
			p_DataSize = ftell(fp);

			// ファイルポインタの位置をファイルの始めに移動
			fseek(fp, 0, SEEK_SET);
			p_DataStartPoint = new char[p_DataSize];

			// 現在のファイル位置の値を取得
			fread_s(p_DataStartPoint, p_DataSize, p_DataSize, 1, fp);

			// 開いたファイルを閉じる
			fclose(fp);

			return p_DataSize;
		}

		// シェーダ作成
		bool Base::Create(ID3D11Device* p_Device_, const char* p_FileName_)
		{
			p_DataSize = LoadFile(p_FileName_);

			if (p_DataSize == 0) return false;

			return true;
		}
	};
};
