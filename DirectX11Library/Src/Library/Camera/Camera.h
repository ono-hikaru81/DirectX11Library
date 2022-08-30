
#ifndef CAMERA_H
#define CAMERA_H

#include "../Graphics/DirectGraphics.h"
#include "../../Utility/Vector.h"

namespace Engine
{
	/*
	* カメラを実装するクラス
	*/
	class Camera
	{
	public:
		/*
		* @breif コンストラクタ
		*/
		Camera() = default;

		/*
		* @breif デストラクタ
		*/
		~Camera() = default;

	public:
		/*
		* @breif アクセサ関数
		* @param pos_ : 座標
		*/
		void SetPos(Utility::Vector& pos_) { pos = pos_; }

		/*
		* @breif アクセサ関数
		* @return カメラ座標
		*/
		Utility::Vector GetPos() { return pos; }

		/*
		* @breif アクセサ関数
		* @param targetPos_ : 注視点座標
		*/
		void SetTargetPos(Utility::Vector& targetPos_) { targetPos_ = targetPos_; }

		/*
		* @breif アクセサ関数
		* @return 注視点座標
		*/
		Utility::Vector GetTargetPos() { return targetPos; }

	public:
		/*
		* @breif 初期化
		* @param p_DirectGraphics_ : DirectGraphicsクラス
		*/
		void Initialize(DirectGraphics* p_DirectGraphics_);

		/*
		* @breif 更新
		* @param p_DirectGraphics_ : DirectGraphicsクラス
		* @param targetPos_ : 注視点座標
		* @param targetDegree_ : 注視点の角度
		*/
		void Update(DirectGraphics* p_DirectGraphics_, Utility::Vector targetPos_, Utility::Vector targetDegree_);

	private:
		Utility::Vector pos { 0.0f, 20.0f, -50.0f };	//! カメラ座標
		Utility::Vector targetPos { 0.0f, 0.0f, 0.0f };	//! 注視点座標
	};
}

#endif // !CAMERA_H
