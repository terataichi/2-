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
};

