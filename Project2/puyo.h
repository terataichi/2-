#pragma once
#include <map>
#include "common/Vector2.h"
#include "INPUT_ID.h"

class puyo
{
public:
	puyo();
	~puyo();
	void UpDate(void);
	void Move(INPUT_ID id);
	void Draw(void);
private:
	void Init(void);

	const Vector2 _size;				// ぷよのサイズ
	const Vector2 _rad;					// ぷよの半径
	std::map<INPUT_ID, Vector2> _vec;	// キーに対しての移動量格納
	Vector2 _pos;
};

