#pragma once

// ΓέΜίΪ°Δ»΅ά
template<class T> class Vector2Template
{
public:
	Vector2Template();
	Vector2Template(T x, T y);
	~Vector2Template();
	T x;
	T y;

	// γόZq
	Vector2Template& operator = (const Vector2Template& vec);

	// δrZq
	bool operator == (const Vector2Template& vec)const;
	bool operator != (const Vector2Template& vec)const;
	bool operator > (const Vector2Template& vec)const;
	bool operator >= (const Vector2Template& vec)const;
	bool operator < (const Vector2Template& vec)const;
	bool operator <= (const Vector2Template& vec)const;
	
	// Y¦Zq
	int& operator [] (int i);

	// PZq
	Vector2Template& operator += (const Vector2Template& vec);
	Vector2Template& operator -= (const Vector2Template& vec);
	Vector2Template& operator *= (T k);
	Vector2Template& operator /= (T k);

	Vector2Template operator + ()const;
	Vector2Template operator - ()const;
};

// ΝήΈΔΩΜZ

// Vector2 + int
template<class T>
Vector2Template<T> operator +(const Vector2Template<T>& u, const T& k);
// Vector2 - int
template<class T>
Vector2Template<T> operator -(const Vector2Template<T>& u, const T& k);
// Vector2 * int
template<class T>
Vector2Template<T> operator *(const Vector2Template<T>& u, const T& k);
// Vector2 / int
template<class T>
Vector2Template<T> operator /(const Vector2Template<T>& u, const T& k);
// Vector2 % int
template<class T>
Vector2Template<T> operator %(const Vector2Template<T>& u, const T& k);

// Vector2 + Vector2
template<class T>
Vector2Template<T> operator +(const Vector2Template<T>& u, const Vector2Template<T>& v);
// Vector2 - Vector2
template<class T>
Vector2Template<T> operator -(const Vector2Template<T>& u, const Vector2Template<T>& v);
// Vector2 * Vector2
template<class T>
Vector2Template<T> operator *(const Vector2Template<T>& u, const Vector2Template<T>& v);
// Vector2 / Vector2
template<class T>
Vector2Template<T> operator /(const Vector2Template<T>& u, const Vector2Template<T>& v);
// Vector2 % Vector2
template<class T>
Vector2Template<T> operator %(const Vector2Template<T>& u, const Vector2Template<T>& v);

// int * Vector2
template<class T>
Vector2Template<T> operator *(const T& k, const Vector2Template<T>& u);

// βΞl
template<class T>
Vector2Template<T> abs(const Vector2Template<T>& u);

using Vector2 = Vector2Template<int>;
using Vector2d = Vector2Template<double>;
using Vector2f = Vector2Template<float>;

#include "./details/Vector2.h"