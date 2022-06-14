
#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

namespace Utility
{
	/**
	* @breif ベクトルクラス
	*/
	class Vector
	{
	public:
		/**
		* @breif コンストラクタ
		*/
		Vector() : x(0), y(0), z(0) {}
		Vector(const float& x_, const float& y_) : x(x_), y(y_), z(0) {}
		Vector(const float& x_, const float& y_, const float& z_) : x(x_), y(y_), z(z_) {}

		/**
		* @breif デストラクタ
		*/
		~Vector() = default;

	public:
		/**
		* @breif アクセサ関数
		*/
		float GetX() const { return x; }
		float GetY() const { return y; }
		float GetZ() const { return z; }

		void Set(const float& x_, const float& y_, const float& z_) { x = x_; y = y_; z = z_; }
		void SetX(const float& x_) { x = x_; }
		void SetY(const float& y_) { y = y_; }
		void SetZ(const float& z_) { z = z_; }

	public:
		//! 加算
		Vector operator+(const Vector& vec_)
		{
			Vector vec { x + vec_.x, y + vec_.y ,z + vec_.z };

			return vec;
		}
		Vector& operator+=(const Vector& vec_)
		{
			x += vec_.x;
			y += vec_.y;
			z += vec_.z;

			return *this;
		}
		//! 減算
		Vector operator-(const Vector& vec_)
		{
			Vector vec { x - vec_.x, y - vec_.y ,z - vec_.z };

			return vec;
		}
		Vector& operator-=(const Vector& vec_)
		{
			x -= vec_.x;
			y -= vec_.y;
			z -= vec_.z;

			return *this;
		}
		//! 乗算
		Vector operator*(const float& value_)
		{
			Vector vec { x * value_, y + value_ ,z + value_ };

			return vec;
		}
		Vector& operator*=(const float& value_)
		{
			x *= value_;
			y *= value_;
			z *= value_;

			return *this;
		}
		//! 除算
		Vector operator/(const float& value_)
		{
			Vector vec { x / value_, y / value_ ,z / value_ };

			return vec;
		}
		Vector& operator/=(const float& value_)
		{
			x /= value_;
			y /= value_;
			z /= value_;

			return *this;
		}
		//! 外積
		Vector operator*(const Vector& vec_)
		{
			Vector vec{ ((y * vec_.z) - (z * vec_.y)), ((z * vec_.x) - (x * vec_.z)), ((x * vec_.y) - (y * vec_.x)) };

			return vec;
		}
		//! 内積
		float operator%(const Vector& vec_)
		{
			float inner{ (x * vec_.GetX()) + (y * vec_.GetY()) + (z * vec_.GetZ()) };

			return inner;
		}
		//! ノルム
		float Norm() const
		{
			float norm{ sqrt((x * x) + (y * y) + (z * z)) };

			return norm;
		}
		//! 正規化
		Vector Normalize() const
		{
			Vector vec { *this };

			vec /= vec.Norm();

			return vec;
		}

	private:
		//! ベクトル成分
		float x;
		float y;
		float z;
	};
}

#endif // !VECTOR_H
