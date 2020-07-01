#include "Vector2.h"

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::~Vector2()
{
}

Vector2& Vector2::operator=(const Vector2& vec)
{
	// TODO: return ステートメントをここに挿入します
	//this->x
	 x = vec.x;
	 y = vec.y;

	 return *this;
}

int& Vector2::operator[](int no)
{
	if (no == 0)
	{
		return x;
	}
	else if (no == 1)
	{
		return y;
	}
	else
	{
		return x;	// 例外処理
	}
}

bool Vector2::operator==(const Vector2& vec) const
{
	return ((x == vec.x) && (y == vec.y));
}

bool Vector2::operator!=(const Vector2& vec) const
{
	return !((x == vec.x) && (y == vec.y));
}

bool Vector2::operator>(const Vector2& vec) const
{
	return ((x > vec.x) && (y > vec.y));
}

bool Vector2::operator>=(const Vector2& vec) const
{
	return ((x >= vec.x) && (y >= vec.y));
}

bool Vector2::operator<(const Vector2& vec) const
{
	return ((x > vec.x) && (y > vec.y));
}

bool Vector2::operator<=(const Vector2& vec) const
{
	return ((x >= vec.x) && (y >= vec.y));
}

Vector2& Vector2::operator+=(const Vector2& vec)
{
	x += vec.x;
	y += vec.y;
	return  *this;
}

Vector2& Vector2::operator-=(const Vector2& vec)
{
	x -= vec.x;
	y -= vec.y;
	return  *this;

}

Vector2& Vector2::operator*=(int k)
{
	x *= k;
	y *= k;
	return *this;
}

Vector2& Vector2::operator/=(int k)
{
	x /= k;
	y /= k;
	return *this;
}

Vector2 Vector2::operator+() const
{
	return *this;
}

Vector2 Vector2::operator-() const
{
	return Vector2(-this->x,-this->y);
}

Vector2::Vector2(int x, int y)
{
	Vector2::x = x;
	Vector2::y = y;
}

Vector2 operator+(const Vector2& u, const int& k)
{
	return Vector2(u.x + k, u.y + k);
}

Vector2 operator-(const Vector2& u, const int& k)
{
	return Vector2(u.x - k, u.y - k);
}

Vector2 operator*(const Vector2& u, const int& k)
{
	return Vector2(u.x * k, u.y * k);
}

Vector2 operator/(const Vector2& u, const int& k)
{
	return Vector2(u.x / k, u.y / k);
}

Vector2 operator%(const Vector2& u, const int& k)
{
	return Vector2(u.x % k, u.y % k);
}

Vector2 operator+(const Vector2& u, const Vector2& v)
{
	return Vector2(u.x + v.x, u.y + v.y);
}

Vector2 operator-(const Vector2& u, const Vector2& v)
{
	return Vector2(u.x - v.x, u.y - v.y);
}

Vector2 operator*(const Vector2& u, const Vector2& v)
{
	return Vector2(u.x * v.x, u.y * v.y);
}

Vector2 operator/(const Vector2& u, const Vector2& v)
{
	return Vector2(u.x / v.x, u.y / v.y);
}

Vector2 operator%(const Vector2& u, const Vector2& v)
{
	return Vector2(u.x % v.x, u.y % v.y);
}
