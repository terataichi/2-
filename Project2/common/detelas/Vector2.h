#include "..\Vector2.h"
#include <cmath>

template<class T>
Vector2Template<T>::Vector2Template()
{
	x = 0;
	y = 0;
}

template<class T>
Vector2Template<T>::Vector2Template(T x, T y)
{
	this-> x = x;
	this-> y = y;
}

template<class T>
Vector2Template<T>::~Vector2Template()
{

}

// ë„ì¸
template<class T>
Vector2Template<T> & Vector2Template<T>::operator=(const Vector2Template<T> & vec)
{
	x = vec.x;
	y = vec.y;
	return *this;
}

// î‰ärââéZéq
template<class T>
bool Vector2Template<T>::operator==(const Vector2Template<T> & vec) const
{
	return ((x == vec.x) && (y == vec.y));
}

template<class T>
bool Vector2Template<T>::operator!=(const Vector2Template<T> & vec) const
{
	return !((x == vec.x) && (y == vec.y));
}

template<class T>
bool Vector2Template<T>::operator>(const Vector2Template<T> & vec) const
{
	return ((x > vec.x )&&(y > vec.y));
}

template<class T>
bool Vector2Template<T>::operator>=(const Vector2Template<T> & vec) const
{
	return ((x >= vec.x) && (y >= vec.y));
}

template<class T>
bool Vector2Template<T>::operator<(const Vector2Template<T> & vec) const
{
	return ((x > vec.x) && (y > vec.y));
}

template<class T>
bool Vector2Template<T>::operator<=(const Vector2Template<T> & vec) const
{
	return ((x >= vec.x) && (y >= vec.y));
}

// ìYÇ¶éöââéZ
template<class T>
T & Vector2Template<T>::operator[](T i)
{
	if (i == 0)
	{
		return x;
	}
	else if (i == 1)
	{
		return y;
	}
	else
	{
		return x;	// ó·äOèàóù
	}
}

// ---íPçÄââéZéq
template<class T>
Vector2Template<T> & Vector2Template<T>::operator+=(const Vector2Template<T> & vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}

template<class T>
Vector2Template<T> & Vector2Template<T>::operator-=(const Vector2Template<T> & vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

template<class T>
Vector2Template<T> & Vector2Template<T>::operator*=(T k)
{
	x *= k;
	y *= k;
	return *this;
}

template<class T>
Vector2Template<T> & Vector2Template<T>::operator/=(T k)
{
	x /= k;
	y /= k;
	return *this;
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator+() const
{
	return *this;
}

template<class T>
Vector2Template<T> Vector2Template<T>::operator-() const
{
	return Vector2Template<T>(-this->x, -this->y);
}

// ---Õﬁ∏ƒŸÇÃââéZ

template<class T>
Vector2Template<T> operator+(const Vector2Template<T>& u, const T& k)
{
	Vector2Template<T> vec;
	vec.x = u.x + k;
	vec.y = u.y + k;

	return vec;
}
template<class T>
Vector2Template<T> operator-(const Vector2Template<T> & u, const T & k)
{
	Vector2Template<T> vec;
	vec.x = u.x - k;
	vec.y = u.y - k;

	return vec;
}
template<class T>
Vector2Template<T> operator%(const Vector2Template<T> & u, const T & k)
{
	Vector2Template<T> vec;
	vec.x = u.x % k;
	vec.y = u.y % k;

	return vec;
}
template<class T>
Vector2Template<T> operator*(const Vector2Template<T> & u, const T & k)
{
	Vector2Template<T> vec;
	vec.x = u.x * k;
	vec.y = u.y * k;

	return vec;
}
template<class T>
Vector2Template<T> operator/(const Vector2Template<T> & u, const T & k)
{
	Vector2Template<T> vec;
	vec.x = u.x / k;
	vec.y = u.y / k;

	return vec;
}

template<class T>
Vector2Template<T> operator+(const Vector2Template<T> & u, const Vector2Template<T> & v)
{
	Vector2Template<T> vec;
	vec.x = u.x + v.x;
	vec.y = u.y + v.y;

	return vec;
}
template<class T>
Vector2Template<T> operator-(const Vector2Template<T> & u, const Vector2Template<T> & v)
{
	Vector2Template<T> vec;
	vec.x = u.x - v.x;
	vec.y = u.y - v.y;

	return vec;
}
template<class T>
Vector2Template<T> operator*(const Vector2Template<T> & u, const Vector2Template<T> & v)
{
	Vector2Template<T> vec;
	vec.x = u.x * v.x;
	vec.y = u.y * v.y;

	return vec;
}
template<class T>
Vector2Template<T> operator/(const Vector2Template<T> & u, const Vector2Template<T> & v)
{
	Vector2Template<T> vec;
	vec.x = u.x / v.x;
	vec.y = u.y / v.y;

	return vec;
}
template<class T>
Vector2Template<T> operator%(const Vector2Template<T> & u, const Vector2Template<T> & v)
{
	Vector2Template<T> vec;
	vec.x = u.x % v.x;
	vec.y = u.y % v.y;

	return vec;
}

template<class T>
Vector2Template<T> operator*(const T & k, const Vector2Template<T> & u)
{
	Vector2Template<T> vec;
	vec.x = k * u.x;
	vec.y = k * u.y;

	return vec;
}

template<class T>
Vector2Template<T> abs(const Vector2Template<T>& u)
{
	return { abs(u.x), abs(u.y) };
}
