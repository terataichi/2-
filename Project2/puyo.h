#pragma once
#include "common/Vector2.h"

class puyo
{
public:
	puyo();
	~puyo();
	void UpDate(void);
	void Move(const Vector2& vec);
	void Draw(void);
private:
	void Init(void);
	Vector2 _pos;
};

