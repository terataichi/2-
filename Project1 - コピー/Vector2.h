#pragma once

class Vector2
{
public:
	Vector2();
	Vector2(int x, int y);
	~Vector2();
	int x;
	int y;

	// ‘ã“ü‰‰ŽZŽq
	Vector2& operator = (const Vector2& vec);
	int& operator[](int no);

	// ”äŠr‰‰ŽZŽq
	bool operator == (const Vector2& vec)const;
	bool operator != (const Vector2& vec)const;
	bool operator > (const Vector2& vec)const;
	bool operator >= (const Vector2& vec)const;
	bool operator < (const Vector2& vec)const;
	bool operator <= (const Vector2& vec)const;

	// ’P€‰‰ŽZŽq
	Vector2& operator += (const Vector2& vec);
	Vector2& operator -= (const Vector2& vec);
	Vector2& operator *= (int k);
	Vector2& operator /= (int k);

	Vector2 operator + ()const;
	Vector2 operator - ()const;
};
//// Vector2 + int
//template<class T>
//Vector2 operator +(const Vector2& u, const int& k);
//// Vector2 - int
//template<class T>
//Vector2 operator -(const Vector2& u, const int& k);
//// Vector2 * int
//template<class T>
//Vector2 operator *(const Vector2& u, const int& k);
//// Vector2 / int
//template<class T>
//Vector2 operator /(const Vector2& u, const int& k);
//// Vector2 % int
//template<class T>
//Vector2 operator %(const Vector2& u, const int& k);

// Vector2 + int
Vector2 operator +(const Vector2& u, const int& k);
// Vector2 - int
Vector2 operator -(const Vector2& u, const int& k);
// Vector2 * int
Vector2 operator *(const Vector2& u, const int& k);
// Vector2 / int
Vector2 operator /(const Vector2& u, const int& k);
// Vector2 % int
Vector2 operator %(const Vector2& u, const int& k);

// Vector2 + Vector2
Vector2 operator +(const Vector2& u, const Vector2& v);
// Vector2 - Vector2
Vector2 operator -(const Vector2& u, const Vector2& v);
// Vector2 * Vector2
Vector2 operator *(const Vector2& u, const Vector2& v);
// Vector2 / Vector2
Vector2 operator /(const Vector2& u, const Vector2& v);
// Vector2 % Vector2
Vector2 operator %(const Vector2& u, const Vector2& v);

//// int * Vector2
//Vector2Template<T> operator *(const T& k, const Vector2Template<T>& u);
