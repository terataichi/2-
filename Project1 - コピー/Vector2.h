#pragma once

class Vector2
{
public:
	Vector2();
	Vector2(int x, int y);
	~Vector2();
	int x;
	int y;

	// ������Z�q
	Vector2& operator = (const Vector2& vec);
	int& operator[](int no);

	// ��r���Z�q
	bool operator == (const Vector2& vec)const;
	bool operator != (const Vector2& vec)const;
	bool operator > (const Vector2& vec)const;
	bool operator >= (const Vector2& vec)const;
	bool operator < (const Vector2& vec)const;
	bool operator <= (const Vector2& vec)const;

	// �P�����Z�q
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
