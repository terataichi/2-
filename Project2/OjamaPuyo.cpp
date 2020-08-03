#include "OjamaPuyo.h"


OjamaPuyo::OjamaPuyo(Vector2&& pos, PuyoID id)
{
	Init(pos,id);
	softCntMax_ = 0;
	seiretuCnt_ = 0;
	seiretu_ = false;
}

OjamaPuyo::~OjamaPuyo()
{
}

void OjamaPuyo::seiretuCnt(int cnt)
{
	seiretuCnt_ = cnt;
}

bool OjamaPuyo::UpDate(void)
{
	// 整列しきっていたらこっち回そう
	if (seiretu_)
	{
		return 	puyo::UpDate();
	}

	//// 整列させる
	if (pos_.x < )
	{
		pos_.x += 4;
	}

	if (pos_.x > )
	{
		pos_.x -= 4;
	}
	
	return false;
}

//bool OjamaPuyo::UpDate()
//{
//	return false;
//}
