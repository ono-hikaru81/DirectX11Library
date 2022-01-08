
#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

namespace Utility
{
	//! ベクトルクラス
	class Vector
	{
	public:
		//! コンストラクタ
		Vector() : x(0), y(0), z(0) {}
		Vector(const double& x_, const double& y_) : x(x_), y(y_), z(0) {}
		Vector(const double &x_, const double& y_, const double& z_) : x(x_), y(y_), z(z_) {}

		~Vector() = default;

	public:
		//! アクセサ関数
		double GetX() const { return x; }
		double GetY() const { return y; }
		double GetZ() const { return z; }

		void Set(const double& x_, const double& y_, const double& z_) { x = x_; y = y_; z = z_; }
		void SetX(const double& x_) { x = x_; }
		void SetY(const double& y_) { y = y_; }
		void SetZ(const double& z_) { z = z_; }

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
		Vector operator*(const double& value_)
		{
			Vector vec { x * value_, y + value_ ,z + value_ };

			return vec;
		}
		Vector& operator*=(const double& value_)
		{
			x *= value_;
			y *= value_;
			z *= value_;

			return *this;
		}
		//! 除算
		Vector operator/(const double& value_)
		{
			Vector vec { x / value_, y / value_ ,z / value_ };

			return vec;
		}
		Vector& operator/=(const double& value_)
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
		double operator%(const Vector& vec_)
		{
			double inner{ (x * vec_.GetX()) + (y * vec_.GetY()) + (z * vec_.GetZ()) };

			return inner;
		}
		//! ノルム
		double Norm() const
		{
			double norm{ sqrt((x * x) + (y * y) + (z * z)) };

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
		double x;
		double y;
		double z;
	};
}

#endif // !VECTOR_H
