
#include "VertexShader.h"

namespace Engine
{
	namespace Shader
	{
		// シェーダファイル読み込み
		int Vertex::LoadFile(const std::string& p_FileName_)
		{
			FILE* p_File = nullptr;

			// ファイルを開く
			fopen_s(&p_File, p_FileName_.c_str(), "rb");

			if (p_File == nullptr) return 0;

			// ファイルポインタの位置をファイルの終わりに移動
			fseek(p_File, 0, SEEK_END);
			p_DataSize = ftell(p_File);

			// ファイルポインタの位置をファイルの始めに移動
			fseek(p_File, 0, SEEK_SET);
			p_DataPoint = new char[p_DataSize];

			// 現在のファイル位置の値を取得
			fread_s(p_DataPoint, p_DataSize, p_DataSize, 1, p_File);

			// 開いたファイルを閉じる
			fclose(p_File);

			return p_DataSize;
		}

		// シェーダ作成
		bool Vertex::Create(ID3D11Device* p_Device_, const std::string& p_FileName_)
		{
			p_DataSize = LoadFile(p_FileName_);

			if (p_DataSize == 0) return false;

			if (FAILED(p_Device_->CreateVertexShader(p_DataPoint, p_DataSize, nullptr, &p_VertexShader))) return false;

			return true;
		}
	};
};
