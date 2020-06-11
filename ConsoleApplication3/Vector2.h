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
};

