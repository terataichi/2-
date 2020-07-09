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
	bool IsCheckPos(void);			// ステージの外に出るかチェックする
	void Init(void);

	const Vector2 _size;			// ぷよのサイズ
	const Vector2 _rad;				// ぷよの半径
	Vector2 _pos;
};

