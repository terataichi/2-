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
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
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
		return x;	// ��O����
	}
}

Vector2::Vector2(int x, int y)
{
	Vector2::x = x;
	Vector2::y = y;
}
