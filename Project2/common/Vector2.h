#pragma once

// √›Ãﬂ⁄∞ƒâªÇµÇ‹
template<class T> class Vector2Template
{
public:
	Vector2Template();
	Vector2Template(T x, T y);
	~Vector2Template();
	T x;
	T y;

	// ë„ì¸ââéZéq
	Vector2Template& operator = (const Vector2Template& vec);

	// î‰ärââéZéq
	bool operator == (const Vector2Template& vec)const;
	bool operator != (const Vector2Template& vec)const;
	bool operator > (const Vector2Template& vec)const;
	bool operator >= (const Vector2Template& vec)const;
	bool operator < (const Vector2Template& vec)const;
	bool operator <= (const Vector2Template& vec)const;
	
	// ìYÇ¶éöââéZéq
	T& operator [] (T i);

	// íPçÄââéZéq
	Vector2Template& operator += (const Vector2Template& vec);
	Vector2Template& operator -= (const Vector2Template& vec);
	Vector2Template& operator *= (T k);
	Vector2Template& operator /= (T k);

	Vector2Template operator + ()const;
	Vector2Template operator - ()const;
};

// Õﬁ∏ƒŸÇÃââéZ

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

// ê‚ëŒíl
template<class T>
Vector2Template<T> abs(const Vector2Template<T>& u);

using Vector2 = Vector2Template<int>;
using Vector2dbl = Vector2Template<double>;
using Vector2flt = Vector2Template<float>;

#include "./detelas/Vector2.h"